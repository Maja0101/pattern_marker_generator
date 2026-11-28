#include <algorithm>
#include <cstddef>
#include <deque>

#include <spdlog/spdlog.h>

#include "common/helpers.hpp"
#include "geometry_calculations/ifp_pocket_calculations.hpp"
#include "geometry_calculations/calculations.hpp"
#include "utils/config.hpp"
#include "geometry_calculations/int128.hpp"

namespace pmg::geometry::calculations::utils {

struct HalfPlane {
    int64_t a;
    int64_t b;
    int64_t c;
};

int64_t dot(int64_t a, int64_t b, const Point64 &p) {
    return (a * p.x) + (b * p.y);
}

int64_t divToNearest(int128_t a, int128_t b) {
    if (b < 0) {
        a = -a;
        b = -b;
    }

    return (b >= 0) ? static_cast<int64_t>((a + b / 2) / b) : static_cast<int64_t>((a - b / 2) / b);
}

Point64 intersect(const HalfPlane &h1, const HalfPlane &h2) {
    int128_t det = mul128(h1.a, h2.b) - mul128(h2.a, h1.b);

    int128_t x   = mul128(h1.b, h2.c) - mul128(h2.b, h1.c);
    int128_t y   = mul128(h2.a, h1.c) - mul128(h1.a, h2.c);


    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("det {} x {} y {} intersection {} {}",
            static_cast<int64_t>(det), static_cast<int64_t>(x), static_cast<int64_t>(y),
            divToNearest(x, det), divToNearest(y, det)));
    }

    return {
        .x=divToNearest(x, det),
        .y=divToNearest(y, det)
    };
}

bool isValidIntersection(const HalfPlane &h1, const HalfPlane &h2, const HalfPlane &target) {
    int128_t det = mul128(h1.a, h2.b) - mul128(h2.a, h1.b);

    // Check if the lines are parallel
    if (det == 0) return false;

    int128_t xNum = mul128(h1.b, h2.c) - mul128(h2.b, h1.c);
    int128_t yNum = mul128(h2.a, h1.c) - mul128(h1.a, h2.c);

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("intersection {} {}",
            static_cast<int64_t>(xNum), static_cast<int64_t>(yNum)));
    }
    
    // Check the sign of (target.a * xNum + target.b * yNum + target.c * det) / det >= 0
    int128_t val = target.a * xNum +
                   target.b * yNum +
                   target.c * det;

    if (det < 0) val = -val;

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("val {}", static_cast<int64_t>(val)));
    }

    return val >= 0;
} 

std::vector<Point64> calculateConvexIFP(const std::vector<Point64> &fabricHull, const std::vector<Point64> &variantHull) {
    // Calculate half planes for two convex hulls and find points of intersection to get IFP for convex hulls
    
    size_t n = fabricHull.size();
    std::vector<HalfPlane> halfPlanes;

    for (size_t i = 0; i < n; ++i) {
        Point64 pointA = fabricHull[i];
        Point64 pointB = fabricHull[(i + 1) % n];

        int64_t dx = pointB.x - pointA.x;
        int64_t dy = pointB.y - pointA.y;

        int64_t nx = -dy;
        int64_t ny = dx;

        int64_t c = -((nx * pointA.x) + (ny * pointA.y));

        Point64 point = variantHull[0];

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("i {} A {} {} B {} {} d {} {} n {} {} c {} point {} {}", i, pointA.x, pointA.y, pointB.x, pointB.y, dx, dy, nx, ny, c, point.x, point.y));
        }

        int64_t minProjection = dot(nx, ny, point);

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("projection {} ", minProjection));
        }

        for (size_t j = 1; j < variantHull.size(); ++j) {
            point = variantHull[j];
            int64_t proj = dot(nx, ny, point);

            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO(std::format("j {} point {} {} proj {}", j, point.x, point.y, proj));
            }

            minProjection = std::min(proj, minProjection);
        }

        int64_t cShifted = c + minProjection;
        halfPlanes.emplace_back(nx, ny, cShifted);

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("half plane {} {} {}", nx, ny, cShifted));
        }
    }

    std::deque<HalfPlane> activePlanes;

    for (size_t i = 0; i < n; ++i) {
        const auto& currPlane = halfPlanes[i];

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("i {} current plane {} {} {}", i, currPlane.a, currPlane.b, currPlane.c));
        }

        // Eliminate inactive edges from the end of the queue
        while(activePlanes.size() >= 2 &&
              !isValidIntersection(activePlanes[activePlanes.size() - 2], activePlanes.back(), currPlane)) {
            activePlanes.pop_back();
        }

        // Eliminate inactive edges from the front of the queue
        while(activePlanes.size() >= 2 &&
              !isValidIntersection(activePlanes[0], activePlanes[1], currPlane)) {
            activePlanes.pop_front();
        }

        activePlanes.push_back(currPlane);
    }

    // Polygon wrap-around
    bool changed = true;
    while (changed && activePlanes.size() >= 3) {
        changed = false;
        if (!isValidIntersection(activePlanes[activePlanes.size() - 2], activePlanes.back(), activePlanes.front())) {
            activePlanes.pop_back();
            changed = true;
        }
        if (activePlanes.size() >= 3 &&
            !isValidIntersection(activePlanes[0], activePlanes[1], activePlanes.back())) {
            activePlanes.pop_front();
            changed = true;
        }
    }

    if (activePlanes.size() < 3) return {};

    std::vector<Point64> ifpVertices;
    size_t activeN = activePlanes.size();

    for (size_t i = 0; i < activeN; ++i) {
        const auto& h1 = activePlanes[i];
        const auto& h2 = activePlanes[(i + 1) % activeN];
        Point64 intersection = intersect(h1, h2);

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("i {} h1 {} {} {} h2 {} {} {}", i, h1.a, h1.b, h1.c, h2.a, h2.b, h2.c));
        }

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("vertex {} {}", intersection.x, intersection.y));
        }
        
        ifpVertices.push_back(intersection);

    }

    return ifpVertices;
}

std::vector<Point64> calculateConvexHull(const std::vector<Point64> &polygon) {
    // Andrew's monotone chain convex hull algorithm
    
    auto n = static_cast<int>(polygon.size());
    if (n <= 3) return polygon;

    // Sort the point in the polygon lexicographically
    std::vector<Point64> sorted = polygon;
    std::ranges::sort(sorted);

    // Prepare the output vector
    std::vector<Point64> hull;
    hull.reserve(2UL*n);
    
    // Build lower hull
    for (int i = 0; i < n; ++i) {
        while (hull.size() >= 2 && crossProduct(hull[hull.size() - 2], hull.back(), sorted[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(sorted[i]);
    }

    // Build upper hull
    size_t lowerSize = hull.size();
    for (int i = n - 2; i >= 0; --i) {
        while (hull.size() > lowerSize && crossProduct(hull[hull.size() - 2], hull.back(), sorted[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(sorted[i]);
    }

    // Remove the last point that is the same as the first
    hull.pop_back();

    return hull;
}

} // namespace pmg::geometry::calculations::utils