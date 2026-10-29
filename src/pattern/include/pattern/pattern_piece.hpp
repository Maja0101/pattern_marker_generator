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
    std::vector<geometry::Point64> m_originalPolygon;
    std::vector<PieceVariant> m_allowedVariants;
    int64_t m_seamAllowance, m_cuttingAllowance;
public:
    PatternPiece(int64_t pieceId, 
                std::vector<geometry::Point64> originalPoints, 
                const std::vector<VariantConstraint>& constraints,
                int64_t seamAllowance = 0, int64_t cuttingAllowance = 0);

    const PieceVariant* getVariant(int rotationAngle, bool isMirrored) const noexcept;
    // Clipper2Lib::Paths64 variantsToClipperPaths() const;

    std::vector<PieceVariant>& getVariants() noexcept { return m_allowedVariants; }
    int64_t getId() const noexcept { return m_id; }

    std::string toString() const;
};

} // namespace pmg::pattern 