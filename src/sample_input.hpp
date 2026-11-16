#pragma once

#include <vector>
#include "pattern/pattern_piece.hpp"
#include "pattern/variant_constraint.hpp"

namespace pmg::samples {
    std::vector<pattern::PatternPiece> getSimpleSamplePieces(
        std::vector<pattern::VariantConstraint> defaultConstraints = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 2 }},
        int64_t seamAllowance = 15000, int64_t cuttingAllowance = 5000
    );

    std::vector<pattern::PatternPiece> getPiecesToCheck();
    std::vector<pattern::PatternPiece> getTestPieces();
} // namespace pmg::samples