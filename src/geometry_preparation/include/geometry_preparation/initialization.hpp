#pragma once 

#include <vector>

#include "common/geometry_types.hpp"

namespace pmg::geometry::preparation {

bool isCyclicShift(const std::vector<Point64>& polygonA, const std::vector<Point64>& polygonB);
std::vector<Point64> calculatePolygonWithAllowance(const std::vector<Point64>& polygon, int64_t seamAllowance, int64_t cuttingAllowance);

} // namespace pmg::geometry::preparation