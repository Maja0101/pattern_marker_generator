#pragma once

#include <vector>

#include <clipper2/clipper.h>

#include "pattern/geometry_types.hpp"

namespace pmg::geometry::calculations::utils {

std::vector<pattern::Point64> calculateConvexIFP(const std::vector<pattern::Point64>& fabricHull, const std::vector<pattern::Point64>& variantHull);
std::vector<pattern::Point64> calculateConvexHull(const std::vector<pattern::Point64>& polygon);

} // namespace pmg::geometry::calculations::utils