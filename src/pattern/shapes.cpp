#include <format>
#include <numeric>
#include <ranges>

#include "pattern/shapes.hpp"

namespace pmg::pattern {

Shape::Shape(std::vector<geometry::Point64> initalBoundary)
    : boundary(std::move(initalBoundary)) {}

Clipper2Lib::Path64 Shape::toClipperPath() const
{
    Clipper2Lib::Path64 path;
    path.reserve(boundary.size());
    for (const auto& point : boundary) {
        path.push_back({point.x, point.y});
    }
    return path;
}

std::string Shape::toString() const
{
    std::string boundaryStr = std::accumulate(
        std::next(boundary.begin()), boundary.end(),
        boundary.front().toString(),
        [](std::string a, geometry::Point64 b) {
            return a + ", " + b.toString();
        }
    );

    return std::format("boundary: ({})", boundaryStr);
}

std::string PieceVariant::toString() const 
{
    std::string boundaryStr = std::accumulate(
        std::next(boundary.begin()), boundary.end(),
        boundary.front().toString(),
        [](std::string a, geometry::Point64 b) {
            return a + ", " + b.toString();
        }
    );

    return std::format("rotationAngle: {}, isMirrored: {}, requiredQuantity: {}, boundary: ({})",
                        rotationAngle,
                        isMirrored,
                        requiredQuantity,
                        boundaryStr);
}

} // namespace pmg::pattern