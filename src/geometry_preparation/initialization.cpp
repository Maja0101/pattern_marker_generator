#include <clipper2/clipper.h>

#include "geometry_preparation/initialization.hpp"
#include "common/helpers.hpp"

namespace pmg::geometry::preparation {

bool isCyclicShift(const std::vector<Point64> &polygonA, const std::vector<Point64> &polygonB)
{
    // Check whether the second polygon is the same as the first polygon
    // even if the starting point does not match

    if (polygonA.size() != polygonB.size()) return false;
    if (polygonA.empty()) return true;

    for (auto it = std::ranges::find(polygonA, polygonB[0]); 
         it != polygonA.end(); 
         it = std::ranges::find(it + 1, polygonA.end(), polygonB[0])) {

        const auto SPLIT = std::distance(polygonA.begin(), it);
        const auto TAIL_LEN = polygonA.size() - SPLIT;

        if (std::ranges::equal(it, polygonA.end(), polygonB.begin(), polygonB.begin() + TAIL_LEN) &&
            std::ranges::equal(polygonA.begin(), it, polygonB.begin() + TAIL_LEN, polygonB.end())) {
                return true;
            }
    }

    return false;
}

std::vector<Point64> calculatePolygonWithAllowance(const std::vector<Point64>& polygon, int64_t seamAllowance, int64_t cuttingAllowance)
{
    Clipper2Lib::Paths64 offsetPaths = Clipper2Lib::InflatePaths(
        { toClipperPath(polygon) },
        static_cast<double>(seamAllowance + cuttingAllowance),
        Clipper2Lib::JoinType::Miter,
        Clipper2Lib::EndType::Polygon
    );

    return fromClipperPath(offsetPaths[0]);
}

} // namespace pmg::geometry::preparation