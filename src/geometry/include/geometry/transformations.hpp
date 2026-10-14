#pragma once

#include <vector>

#include "geometry_types.hpp"

namespace pmg::geometry {

std::vector<Point64> transformPolygon(const std::vector<Point64> polygon, int rotationAngle, bool isMirrored) noexcept;

} // namespace pmg::geometry