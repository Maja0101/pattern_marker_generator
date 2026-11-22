#pragma once

#include <cstdint>

#include <clipper2/clipper.h>

#include "common/geometry_types.hpp"

namespace pmg::geometry {

int64_t crossProduct(const Point64& a, const Point64& b, const Point64& c);

Clipper2Lib::Path64 toClipperPath(const std::vector<Point64>& polygon);
std::vector<Point64> fromClipperPath(const Clipper2Lib::Path64& path);

std::vector<Point64> transformPolygon(const std::vector<Point64>& polygon, int rotationAngle, bool isMirrored, bool toBeAnchored = true, bool mirroringInFlippedXAxis = true) noexcept;

} // namespace pmg::geometry