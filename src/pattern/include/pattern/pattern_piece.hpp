#pragma once

#include <string>
#include <vector>

#include "clipper2/clipper.h"

#include "geometry/geometry_types.hpp"
#include "shapes.hpp"
#include "variant_constraint.hpp"

namespace pmg::pattern {

class PatternPiece {
private:
    int64_t m_id;
    std::vector<geometry::Point64> m_originalBoundary;
    std::vector<PieceVariant> m_allowedVariants;
public:
    PatternPiece(int pieceId, 
              std::vector<geometry::Point64> originalPoints, 
              const std::vector<VariantConstraint>& constraints);

    const PieceVariant* getVariant(int rotationAngle, bool isMirrored) const noexcept;
    Clipper2Lib::Paths64 variantsToClipperPaths() const;

    const std::vector<PieceVariant>& getVariants() const noexcept { return m_allowedVariants; }
    int64_t getId() const noexcept { return m_id; }
};

} // namespace pmg::pattern 