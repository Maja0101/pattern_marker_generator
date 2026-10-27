#pragma once

#include <vector>

#include <clipper2/clipper.h>

#include "geometry/geometry_types.hpp"
#include "pattern/pattern_piece.hpp"
#include "nesting/bl_state.hpp"
#include "nesting/nested_item.hpp"
#include "pattern/shapes.hpp"

namespace pmg::geometry::calculations::utils {

bool aabbCollision(const Clipper2Lib::Rect64& a, const Clipper2Lib::Rect64& b);
bool isCollision(const nesting::BLStateView& view, const nesting::NestedItem& item, int64_t x, int64_t y);
bool isWithinFabric(const nesting::BLStateView& view, const nesting::NestedItem& item, int64_t x, int64_t y);

int64_t crossProduct(const Point64& a, const Point64& b, const Point64& c);

} // namespace pmg::geometry::calculations::utils

namespace pmg::geometry::calculations {

Clipper2Lib::Point64 toClipperPoint(const Point64& point);
Clipper2Lib::Path64 toClipperPath(const std::vector<Point64>& polygon);
std::vector<Point64> fromClipperPath(const Clipper2Lib::Path64& path);
std::vector<Point64> calculatePolygonWithAllowance(const std::vector<Point64>& polygon, int64_t seamAllowance, int64_t cuttingAllowance);
Clipper2Lib::Paths64 transaltePaths(const Clipper2Lib::Paths64& paths, const Clipper2Lib::Point64& point);
Clipper2Lib::Point64 snapToBoundaries(Clipper2Lib::Point64 point, const Clipper2Lib::Paths64& ifpPaths, const std::vector<Clipper2Lib::Paths64>& originalNfps, int64_t epsilon = 1);

bool canPlaceAt(const nesting::BLStateView& view, const nesting::NestedItem& item, int64_t x, int64_t y);
std::vector<nesting::NestedItem> prepareItems(std::vector<pattern::PatternPiece>& patternPieces);
std::vector<pattern::PieceVariant> getAllVariants(std::vector<pattern::PatternPiece>& patternPieces);
bool evaluate(const std::vector<nesting::NestedItem>& items);

} // namespace pmg::geometry::calculations