#pragma once

#include <vector>

#include "pattern/nesting_result.hpp"
#include "pattern/shapes.hpp"
#include "pattern/pattern_piece.hpp"

namespace pmg::nesting {

pattern::NestingStats checkStatsFor(
    const pattern::Fabric& fabric, 
    std::vector<pattern::PatternPiece>& patternPieces,
    const std::vector<pattern::ItemPlacement> newPlacements
); 

} // namespace pmg::nesting 