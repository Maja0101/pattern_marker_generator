#pragma once

#include <vector>

#include <clipper2/clipper.h>

#include "pattern/geometry_types.hpp"

namespace pmg::geometry::calculations {

pattern::Point64 getTopLeftCorner(const std::vector<pattern::Point64>& polygon) noexcept;
std::vector<pattern::Point64> transformPolygon(const std::vector<pattern::Point64>& polygon, int rotationAngle, bool isMirrored, bool toBeAnchored = true, bool mirroringInFlippedXAxis = true) noexcept;
std::vector<pattern::Point64> rotateBy180(const std::vector<pattern::Point64>& polygon) noexcept;
std::vector<Clipper2Lib::Path64> rotatePathsBy180(const std::vector<Clipper2Lib::Path64>& paths, int64_t offsetX, int64_t offsetY) noexcept;

} // namespace pmg::geometry::calculations