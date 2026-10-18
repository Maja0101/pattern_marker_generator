#include <algorithm>
#include <functional>
#include <unordered_map>
#include <utility>

#include "geometry/decomposition_calculations.hpp"

namespace pmg::geometry::calculations::utils {

struct Edge {
    int u, v;
    
    constexpr Edge(int a, int b) noexcept
        : u(std::min(a, b)), v(std::max(u, v)) {}

    auto operator<=>(const Edge&) const = default;
};

struct HalfEdge {
    int v; // vertex index where edge ends
    int next; // index of the next half edge 
    int prev; // index of the prev half edge 
    int twin = -1; // index of the opposite half edge 
    bool active = true;
};

inline int crossProductSign(const Point64 &a, const Point64 &b, const Point64 &c)
{
    int64_t crossProduct = (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
    return (crossProduct > 0) ? 1 : ((crossProduct < 0) ? -1 : 0);
}

inline bool isConvex(const Point64 &a, const Point64 &b, const Point64 &c)
{
    return crossProductSign(a, b, c) >= 0;
}

} // namespace pmg::geometry::calculations::utils

template<>
struct std::hash<pmg::geometry::calculations::utils::Edge> {
    std::size_t operator()(const pmg::geometry::calculations::utils::Edge& e) const noexcept {
        return (static_cast<size_t>(e.u) << 32 | static_cast<uint32_t>(e.v)); // formula for creating 64-bit hash from two 32-bit integer indexes 
    }
};

namespace pmg::geometry::calculations {

std::vector<std::vector<Point64>> hertelMehlhornAlgorithm(const std::vector<Point64> &polygon)
{
    const size_t n = polygon.size();
    if (n < 3) return {};
    if (n == 3) return { polygon }; 

    // Triangulate the polygon
    std::vector<std::vector<Point64>> input = { polygon };
    std::vector<uint32_t> indexes = mapbox::earcut<uint32_t>(input);
    const size_t trianglesCount = indexes.size() / 3;

    if (trianglesCount == 0) return {};

    // Create DCEL (doubly connected edge list) for fast diagonal checking
    std::vector<utils::HalfEdge> halfEdges;
    halfEdges.reserve(trianglesCount * 3);

    std::vector<int> diagonals;
    diagonals.reserve(trianglesCount);

    std::unordered_map<utils::Edge, int> edgeMap;

    for (size_t t = 0; t < trianglesCount; ++t) {
        int idx0 = static_cast<int>(indexes[t * 3 + 0]);
        int idx1 = static_cast<int>(indexes[t * 3 + 1]);
        int idx2 = static_cast<int>(indexes[t * 3 + 2]);

        // Make sure the vertixes are counterclockwise
        if (utils::crossProductSign(polygon[idx0], polygon[idx1], polygon[idx2]) < 0) {
            std::swap(idx1, idx2);
        }

        int baseIdx = static_cast<int>(halfEdges.size());

        // Create half edges for the triangle
        utils::HalfEdge e0{ idx1, baseIdx + 1, baseIdx + 2}; // idx0 -> idx1
        utils::HalfEdge e1{ idx2, baseIdx + 2, baseIdx + 0}; // idx1 -> idx2
        utils::HalfEdge e2{ idx0, baseIdx + 0, baseIdx + 1}; // idx2 -> idx0

        halfEdges.push_back(e0);
        halfEdges.push_back(e1);
        halfEdges.push_back(e2);

        // Connect twin half edges
        int vertexPairs[3][2] = {
            {idx0, idx1},
            {idx1, idx2},
            {idx2, idx0}
        };

        for (int i = 0; i < 3; ++i) {
            int edgeIdx = baseIdx + 1;
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

        int A = e2.v;
        int B = e1.v;
        int C = halfEdges[fromB].v;
        int D = halfEdges[fromA].v;

        if (utils::isConvex(polygon[C], polygon[A], polygon[D]) &&
            utils::isConvex(polygon[D], polygon[B], polygon[C])) {
                halfEdges[toA].next = fromA;
                halfEdges[fromA].prev = toA;

                halfEdges[toB].next = fromB;
                halfEdges[fromB].prev = toB;

                e1.active = false;
                e2.active = false;
        }
    }

    // Construct the decomposed polygon
    std::vector<std::vector<Point64>> result;
    std::vector<bool> visited(halfEdges.size(), false);

    for (size_t i = 0; i < halfEdges.size(); ++i) {
        if (!halfEdges[i].active || visited[i]) continue;

        std::vector<Point64> poly;
        int currEdgeIdx = static_cast<int>(i);

        while (!visited[currEdgeIdx]) {
            visited[currEdgeIdx] = true;

            poly.push_back(polygon[halfEdges[currEdgeIdx].v]);

            currEdgeIdx = halfEdges[currEdgeIdx].next;
        }

        if (poly.size() >= 3) {
            result.push_back(std::move(poly));
        }
    }

    return result;
}

} // namespace pmg::geometry::calculations