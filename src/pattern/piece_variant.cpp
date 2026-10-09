#include <format>
#include <numeric>
#include <ranges>

#include "pattern/piece_variant.hpp"

namespace pmg::pattern {

Clipper2Lib::Path64 PieceVariant::toClipperPath() const
{
    Clipper2Lib::Path64 path;
    path.reserve(transformedBoundary.size());
    for (const auto& point : transformedBoundary) {
        path.push_back({point.x, point.y});
    }
    return path;
}

std::string PieceVariant::toString() const
{
    std::string boundaryStr = std::accumulate(
        std::next(transformedBoundary.begin()), transformedBoundary.end(),
        transformedBoundary.front().toString(),
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