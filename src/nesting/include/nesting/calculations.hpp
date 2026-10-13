#pragma once

#include <vector>

#include <clipper2/clipper.h>

#include "pattern/pattern_piece.hpp"
#include "nesting/bl_state.hpp"
#include "nesting/nested_item.hpp"

namespace pmg::nesting::calculations {

bool aabbCollision(const Clipper2Lib::Rect64& a, const Clipper2Lib::Rect64& b);
bool isCollision(const BLStateView& view, const NestedItem& item, int64_t x, int64_t y);
bool isWithinFabric(const BLStateView& view, const NestedItem& item, int64_t x, int64_t y);
bool canPlaceAt(const BLStateView& view, const NestedItem& item, int64_t x, int64_t y);

std::vector<nesting::NestedItem> prepareItems(const std::vector<pattern::PatternPiece>& patternPieces);
bool evaluate(const std::vector<nesting::NestedItem>& items);

} // namespace pmg::nesting::calculations