#pragma once

#include <vector>

#include <clipper2/clipper.h>

#include "geometry/geometry_types.hpp"

namespace pmg::geometry::calculations::utils {

std::vector<Point64> calculateConvexHull(const std::vector<Point64>& polygon);

} // namespace pmg::geometry::calculations::utils