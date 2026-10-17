#pragma once

#include <vector>

#include <clipper2/clipper.h>

#include "geometry/geometry_types.hpp"
#include "pattern/pattern_piece.hpp"
#include "nesting/bl_state.hpp"
#include "nesting/nested_item.hpp"

namespace pmg::geometry::calculations {
Clipper2Lib::Path64 toClipperPath(const std::vector<geometry::Point64>& polygon);
std::vector<geometry::Point64> fromClipperPath(const Clipper2Lib::Path64& path);

std::vector<geometry::Point64> calculatePolygonWithAllowance(const std::vector<geometry::Point64>& polygon, int64_t seamAllowance, int64_t cuttingAllowance);

bool aabbCollision(const Clipper2Lib::Rect64& a, const Clipper2Lib::Rect64& b);
bool isCollision(const nesting::BLStateView& view, const nesting::NestedItem& item, int64_t x, int64_t y);
bool isWithinFabric(const nesting::BLStateView& view, const nesting::NestedItem& item, int64_t x, int64_t y);
bool canPlaceAt(const nesting::BLStateView& view, const nesting::NestedItem& item, int64_t x, int64_t y);

std::vector<nesting::NestedItem> prepareItems(const std::vector<pattern::PatternPiece>& patternPieces);
bool evaluate(const std::vector<nesting::NestedItem>& items);

bool isAARectangle(const Clipper2Lib::Path64& path, const Clipper2Lib::Rect64& boundingBox);
Clipper2Lib::Path64 invertPath(const Clipper2Lib::Path64& path);

} // namespace pmg::geometry::calculations