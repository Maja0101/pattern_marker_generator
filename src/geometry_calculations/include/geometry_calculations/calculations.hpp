#pragma once

#include <vector>

#include <clipper2/clipper.h>

#include "common/geometry_types.hpp"
#include "pattern/pattern_piece.hpp"
#include "pattern/nested_item.hpp"
#include "pattern/nesting_result.hpp"
#include "pattern/shapes.hpp"

namespace pmg::geometry::calculations {

Clipper2Lib::Point64 toClipperPoint(const Point64& point);

Clipper2Lib::Paths64 transaltePaths(const Clipper2Lib::Paths64& paths, const Clipper2Lib::Point64& point);
Point64 rotatePointCcwBy(const Point64& point, int angle);
Point64 rotatePointCwBy(const Point64& point, int angle);

Clipper2Lib::Point64 snapToBoundaries(Clipper2Lib::Point64 point, const Clipper2Lib::Paths64& ifpPaths, const std::vector<Clipper2Lib::Paths64>& originalNfps, int64_t epsilon = 1);

std::vector<pattern::NestedItem> prepareItems(std::vector<pattern::PatternPiece>& patternPieces);
std::vector<pattern::PieceVariant> getAllVariants(std::vector<pattern::PatternPiece>& patternPieces);

double calculateSolidity(const Clipper2Lib::Path64& scrapPath);
pattern::NestingStats evaluate(const Clipper2Lib::Paths64& allPlacedItems, const pattern::Fabric& fabric, int64_t maxRightPoint);

pattern::NestingStats recalculateStats(
    const pattern::Fabric& fabric, 
    std::vector<pattern::NestedItem>& nestedItems,
    const std::vector<pattern::ItemPlacement>& newPlacements
); 

} // namespace pmg::geometry::calculations