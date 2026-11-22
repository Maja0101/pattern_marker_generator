#pragma once 

#include <vector>

#include <mapbox/earcut.hpp>

#include "pattern/geometry_types.hpp"

namespace pmg::geometry::calculations {

bool checkConvexity(const std::vector<pattern::Point64>& polygon);
std::vector<std::vector<pattern::Point64>> hertelMehlhornAlgorithm(const std::vector<pattern::Point64>& polygon);

} // namespace pmg::geometry::calculations