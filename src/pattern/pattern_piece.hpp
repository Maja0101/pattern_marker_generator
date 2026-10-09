#pragma once

#include <string>
#include <vector>

// #include <clipper2/clipper.h>

#include "geometry/geometry_types.hpp"
#include "piece_variant.hpp"
#include "variant_constraint.hpp"

namespace pmg::pattern {

class PatternPiece {
private:
    int64_t id;
    std::vector<geometry::Point64> originalBoundary;
    std::vector<PieceVariant> allowedVariants;
public:
    PatternPiece(int pieceId, 
              std::vector<geometry::Point64> originalPoints, 
              const std::vector<VariantConstraint>& constraints);

    const PieceVariant* getVariant(int rotationAngle, bool isMirrored) const noexcept;

    const std::vector<PieceVariant>& getVariants() const noexcept { return allowedVariants; }
    int64_t getId() const noexcept { return id; }
};

} // namespace pmg::pattern 