#pragma once 

#include <vector>

#include <mapbox/earcut.hpp>

#include "geometry/geometry_types.hpp"
#include "pattern/shapes.hpp"

namespace pmg::geometry::calculations {

bool checkConvexity(const std::vector<Point64>& polygon);
std::vector<std::vector<Point64>> hertelMehlhornAlgorithm(const std::vector<Point64>& polygon);

} // namespace pmg::geometry::calculations