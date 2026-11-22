#pragma once 

#include <vector>

#include <mapbox/earcut.hpp>

#include "common/geometry_types.hpp"

namespace pmg::geometry::preparation {

bool checkConvexity(const std::vector<Point64>& polygon);
std::vector<std::vector<Point64>> hertelMehlhornAlgorithm(const std::vector<Point64>& polygon);

} // namespace pmg::geometry::preparation