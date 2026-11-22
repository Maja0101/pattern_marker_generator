#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>
#include <limits>
#include <numbers>

#include <spdlog/spdlog.h>

#include "geometry_preparation/transformations.hpp"
#include "common/helpers.hpp"

namespace pmg::geometry::preparation {

Point64 getTopLeftCorner(const std::vector<Point64> &polygon) noexcept
{
    if (polygon.empty()) return {};

    int64_t minX = std::numeric_limits<int64_t>::max();
    int64_t maxY = std::numeric_limits<int64_t>::min();

    for (const auto& point : polygon) {
        minX = std::min(point.x, minX);
        maxY = std::max(point.y, maxY);
    }

    return Point64{minX, maxY};
}

std::vector<Point64> rotateBy180(const std::vector<Point64> &polygon) noexcept
{
    return transformPolygon(polygon, 180, false);
}

std::vector<Clipper2Lib::Path64> rotatePathsBy180(const std::vector<Clipper2Lib::Path64> &paths, int64_t offsetX, int64_t offsetY) noexcept
{
    std::vector<Clipper2Lib::Path64> rotated;
    rotated.reserve(paths.size());

    for (const auto& path : paths) {
        Clipper2Lib::Path64 rotatedPath;
        rotatedPath.reserve(path.size());

        for (const auto& point : path) {
            rotatedPath.emplace_back(
                (-point.x + offsetX),
                (-point.y + offsetY)
            );
        }

        rotated.push_back(rotatedPath);
    }

    return rotated;
}

} // namespace pmg::geometry::preparation
