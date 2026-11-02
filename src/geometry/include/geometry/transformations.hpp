#pragma once

#include <vector>

#include "clipper2/clipper.h"

#include "geometry_types.hpp"

namespace pmg::geometry::calculations {

std::vector<Point64> transformPolygon(const std::vector<Point64>& polygon, int rotationAngle, bool isMirrored) noexcept;
std::vector<Point64> rotateBy180(const std::vector<Point64>& polygon) noexcept;
std::vector<Clipper2Lib::Path64> rotatePathsBy180(const std::vector<Clipper2Lib::Path64>& paths, int64_t offsetX, int64_t offsetY) noexcept;

} // namespace pmg::geometry::calculations