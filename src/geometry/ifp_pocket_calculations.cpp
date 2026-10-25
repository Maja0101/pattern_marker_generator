#include <spdlog/spdlog.h>

#include "geometry/ifp_pocket_calculations.hpp"
#include "geometry/calculations.hpp"
#include "utils/config.hpp"

namespace pmg::geometry::calculations::utils {
struct HalfPlane {
    int64_t a, b, c;
};

int64_t dot(int64_t a, int64_t b, Point64 &p) {
    return a * p.x + b * p.y;
}

int64_t divToNearest(__int128_t a, __int128_t b) {
    if (b < 0) {
        a = -a;
        b = -b;
    }

    return (b >= 0) ? static_cast<int64_t>((a + b / 2) / b) : static_cast<int64_t>((a - b / 2) / b);
}

Point64 intersect(const HalfPlane &h1, const HalfPlane &h2) {
    __int128_t det = static_cast<__int128_t>(h1.a) * h2.b - static_cast<__int128_t>(h2.a) * h1.b;

    __int128_t x = static_cast<__int128_t>(h1.b) * h2.c - static_cast<__int128_t>(h2.b) * h1.c;
    __int128_t y = static_cast<__int128_t>(h2.a) * h1.c - static_cast<__int128_t>(h1.a) * h2.c;

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("det {} x {} y {} intersection {} {}", det, x, y, divToNearest(x, det), divToNearest(y, det)));
    }

    return {
        divToNearest(x, det),
        divToNearest(y, det)
    };
}

std::vector<Point64> calculateConvexIFP(const std::vector<Point64> &fabricHull, const std::vector<Point64> &variantHull) {
    size_t n = fabricHull.size();
    std::vector<HalfPlane> halfPlanes;

    for (size_t i = 0; i < n; ++i) {
        Point64 A = fabricHull[i];
        Point64 B = fabricHull[(i + 1) % n];

        int64_t dx = B.x - A.x;
        int64_t dy = B.y - A.y;

        int64_t nx = -dy;
        int64_t ny = dx;

        int64_t c = -(nx * A.x + ny * A.y);

        Point64 point = variantHull[0];

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("i {} A {} {} B {} {} d {} {} n {} {} c {} point {} {}", i, A.x, A.y, B.x, B.y, dx, dy, nx, ny, c, point.x, point.y));
        }

        int64_t minProjection = dot(nx, ny, point);
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("proj {} ", minProjection));
        }
        for (size_t j = 1; j < variantHull.size(); ++j) {
            point = variantHull[j];
            int64_t proj = dot(nx, ny, point);
            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO(std::format("j {} point {} {} proj {}", j, point.x, point.y, proj));
            }
            if (proj < minProjection) {
                minProjection = proj;
            }
        }

        int64_t cShifted = c + minProjection;
        halfPlanes.push_back({nx, ny, cShifted});

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("half plane {} {} {}", nx, ny, cShifted));
        }
    }

    std::vector<Point64> ifpVertices;
    for (size_t i = 0; i < n; ++i) {
        const auto& h1 = halfPlanes[i];
        const auto& h2 = halfPlanes[(i + 1) % n];
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("i {} h1 {} {} {} h2 {} {} {}", i, h1.a, h1.b, h1.c, h2.a, h2.b, h2.c));
        }
        ifpVertices.push_back(intersect(h1, h2));
    }

    return ifpVertices;
}

std::vector<Point64> calculateConvexHull(const std::vector<Point64> &polygon) {
    int n = polygon.size();
    if (n <= 3) return polygon;

    // Sort the point in the polygon lexicographically
    std::vector<Point64> sorted = polygon;
    std::sort(sorted.begin(), sorted.end());

    // Prepare the output vector
    std::vector<Point64> hull;
    hull.reserve(2*n);
    
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