#pragma once

#include <vector>
#include "pattern/pattern_piece.hpp"
#include "pattern/variant_constraint.hpp"

namespace pmg::samples {
    std::vector<pattern::PatternPiece> getSimpleSamplePieces(
        std::vector<pattern::VariantConstraint> defaultConstraints = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 2 }}
    );
} // namespace pmg::samples