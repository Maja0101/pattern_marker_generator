#include <algorithm>
#include <functional>
#include <unordered_map>
#include <utility>

#include <spdlog/spdlog.h>

#include "geometry/decomposition_calculations.hpp"
#include "geometry/calculations.hpp"
#include "utils/config.hpp"

namespace mapbox::util {
    template <>
    struct nth<0, pmg::geometry::Point64> {
        static auto get(const pmg::geometry::Point64& p) { return p.x; }
    };
    
    template <>
    struct nth<1, pmg::geometry::Point64> {
        static auto get(const pmg::geometry::Point64& p) { return p.y; }
    };
} // namespace mapbox::util

namespace pmg::geometry::calculations::utils {

struct Edge {
    int u;
    int v;
    
    constexpr Edge(int a, int b) noexcept
        : u(std::min(a, b)), v(std::max(a, b)) {}

    auto operator<=>(const Edge&) const = default;
};

struct HalfEdge {
    int v; // vertex index where edge ends
    int next; // index of the next half edge 
    int prev; // index of the prev half edge 
    int twin = -1; // index of the opposite half edge 
    bool active = true;
};

bool isConvex(const Point64 &a, const Point64 &b, const Point64 &c)
{
    return crossProduct(a, b, c) >= 0;
}

} // namespace pmg::geometry::calculations::utils

template<>
struct std::hash<pmg::geometry::calculations::utils::Edge> {
    std::size_t operator()(const pmg::geometry::calculations::utils::Edge& e) const noexcept {
      const std::uint64_t IDX_COMBINED = 
            (static_cast<std::uint64_t>(static_cast<std::uint32_t>(e.u)) << 32) |
            static_cast<std::uint32_t>(e.v);
      return std::hash<std::uint64_t>{}(
          IDX_COMBINED); // formula for creating 64-bit hash from two 32-bit integer indexes 
    }
};

namespace pmg::geometry::calculations {

bool checkConvexity(const std::vector<Point64> &polygon)
{
    const size_t N = polygon.size();
    if (N < 3) return false;
    if (N == 3) return true;

    for (size_t i = 0; i < N; ++i) {
        const geometry::Point64& p1 = polygon[i];
        const geometry::Point64& p2 = polygon[(i + 1) % N];
        const geometry::Point64& p3 = polygon[(i + 2) % N];

        if (!geometry::calculations::utils::isConvex(p1, p2, p3)) {
            return false;
        }
    }

    return true;
}

std::vector<std::vector<Point64>> hertelMehlhornAlgorithm(const std::vector<Point64> &polygon)
{
    const size_t N = polygon.size();
    if (N < 3) return {};
    if (N == 3) return { polygon };
    if (checkConvexity(polygon)) return { polygon };

    // Triangulate the polygon
    std::vector<std::vector<Point64>> input = { polygon };
    std::vector<uint32_t> indexes = mapbox::earcut<uint32_t>(input);
    const size_t TRIANGLES_COUNT = indexes.size() / 3;

    if (TRIANGLES_COUNT == 0) return {};

    if constexpr (config::DEBUG_MODE_DETAILED){
        SPDLOG_INFO(std::format("trianglesCount: {}", TRIANGLES_COUNT));
    }

    // Create DCEL (doubly connected edge list) for fast diagonal checking
    std::vector<utils::HalfEdge> halfEdges;
    halfEdges.reserve(TRIANGLES_COUNT * 3);

    std::vector<int> diagonals;
    diagonals.reserve(TRIANGLES_COUNT);

    std::unordered_map<utils::Edge, int> edgeMap;

    for (size_t t = 0; t < TRIANGLES_COUNT; ++t) {
        auto idx0 = static_cast<int>(indexes[(t * 3) + 0]);
        auto idx1 = static_cast<int>(indexes[(t * 3) + 1]);
        auto idx2 = static_cast<int>(indexes[(t * 3) + 2]);

        // Make sure the vertixes are counterclockwise
        if (utils::crossProduct(polygon[idx0], polygon[idx1], polygon[idx2]) < 0) {
            std::swap(idx1, idx2);
        }

        auto baseIdx = static_cast<int>(halfEdges.size());

        // Create half edges for the triangle
        utils::HalfEdge e0{ .v=idx1, .next=baseIdx + 1, .prev=baseIdx + 2}; // idx0 -> idx1
        utils::HalfEdge e1{ .v=idx2, .next=baseIdx + 2, .prev=baseIdx + 0}; // idx1 -> idx2
        utils::HalfEdge e2{ .v=idx0, .next=baseIdx + 0, .prev=baseIdx + 1}; // idx2 -> idx0

        halfEdges.push_back(e0);
        halfEdges.push_back(e1);
        halfEdges.push_back(e2);

        // Connect twin half edges
        std::array<std::array<int, 2>, 3> vertexPairs = {{
            {idx0, idx1},
            {idx1, idx2},
            {idx2, idx0}
        }};

        for (int i = 0; i < 3; ++i) {
            int edgeIdx = baseIdx + i;
            utils::Edge eKey(vertexPairs[i][0], vertexPairs[i][1]);

            auto [it, inserted] = edgeMap.try_emplace(eKey, edgeIdx);
            if (!inserted) {
                int twinIdx = it->second;
                halfEdges[edgeIdx].twin = twinIdx;
                halfEdges[twinIdx].twin = edgeIdx;

                diagonals.push_back(edgeIdx);
            }
        }
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("diagonals size: {}", diagonals.size()));
        SPDLOG_INFO(std::format("halfEdges size: {}", halfEdges.size()));
    }

    // Remove non-essential diagonals 
    for (int diagonalIdx : diagonals) {
        utils::HalfEdge& e1 = halfEdges[diagonalIdx];
        if (!e1.active || e1.twin == -1) continue;

        utils::HalfEdge& e2 = halfEdges[e1.twin];
        if (!e2.active) continue;

        // Check convexity for two vertexes of the diagonal
        int toA = e1.prev; // C -> A
        int fromA = e2.next; // A -> D
        int toB = e2.prev; // D -> B
        int fromB = e1.next; // B -> C

        int pointA = e2.v;
        int pointB = e1.v;
        int pointC = halfEdges[fromB].v;
        int pointD = halfEdges[fromA].v;

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("diagonal to check: {} {} {} {}", pointA, pointB, pointC, pointD));
            SPDLOG_INFO(std::format("diagonal to check: {} {} {} {}", toA, fromA, toB, fromB));
        }

        if (utils::isConvex(polygon[pointC], polygon[pointA], polygon[pointD]) &&
            utils::isConvex(polygon[pointD], polygon[pointB], polygon[pointC])) {
                halfEdges[toA].next = fromA;
                halfEdges[fromA].prev = toA;

                halfEdges[toB].next = fromB;
                halfEdges[fromB].prev = toB;

                e1.active = false;
                e2.active = false;

                if constexpr (config::DEBUG_MODE_DETAILED) {
                    SPDLOG_INFO(std::format("non essential: {} {} {} {}", halfEdges[toA].next, halfEdges[fromA].prev, halfEdges[toB].next, halfEdges[fromB].prev));
                }
        }
    }

    // Construct the decomposed polygon
    std::vector<std::vector<Point64>> result;
    std::vector<bool> visited(halfEdges.size(), false);

    for (size_t i = 0; i < halfEdges.size(); ++i) {
        if (!halfEdges[i].active || visited[i]) continue;

        std::vector<Point64> poly;
        auto currEdgeIdx = static_cast<int>(i);

        while (!visited[currEdgeIdx]) {
            visited[currEdgeIdx] = true;

            poly.push_back(polygon[halfEdges[currEdgeIdx].v]);

            currEdgeIdx = halfEdges[currEdgeIdx].next;
        }

        if (poly.size() >= 3) {
            result.push_back(std::move(poly));
        }
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& poly : result) {
            SPDLOG_INFO("polygon");
            for (const auto& p : poly) {
                SPDLOG_INFO(std::format("decomposed point ({}, {})", p.x, p.y));
            }
        }
    }

    return result;
}

} // namespace pmg::geometry::calculations