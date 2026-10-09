#pragma once

#include "geometry_types.hpp"

namespace pmg::geometry {

Point64 transformPoint(Point64 point, int rotationAngle, bool isMirrored) noexcept;

} // namespace pmg::geometry