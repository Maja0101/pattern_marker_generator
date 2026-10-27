#pragma once

#include <vector>

#include "geometry_types.hpp"

namespace pmg::geometry::calculations {

std::vector<Point64> transformPolygon(const std::vector<Point64>& polygon, int rotationAngle, bool isMirrored) noexcept;
std::vector<Point64> rotateBy180(const std::vector<Point64>& polygon) noexcept;

} // namespace pmg::geometry::calculations