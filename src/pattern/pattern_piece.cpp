#include <utility>

#include "pattern/pattern_piece.hpp"
#include "geometry/transformations.hpp"

namespace pmg::pattern {

PatternPiece::PatternPiece(int pieceId, 
              std::vector<geometry::Point64> originalPoints, 
              const std::vector<VariantConstraint>& constraints)
    : m_id(pieceId), m_originalBoundary(std::move(originalPoints)) 
{
    m_allowedVariants.reserve(constraints.size());

    // TODO the transformation of the original boundary into all possible variants should be updated 
    for (const auto& req : constraints) {
        std::vector<pmg::geometry::Point64> transformedBoundary;
        transformedBoundary.reserve(m_originalBoundary.size());

        for (const auto& point: m_originalBoundary) {
            auto transformedPoint = geometry::transformPoint(point, req.rotationAngle, req.isMirrored);
            transformedBoundary.push_back(transformedPoint);
        }

        PieceVariant variant(req.rotationAngle, req.isMirrored, req.requiredQuantity, transformedBoundary);

        m_allowedVariants.push_back(std::move(variant));
    }
}

const PieceVariant* PatternPiece::getVariant(int rotationAngle, bool isMirrored) const noexcept {
    for (const auto& variant : m_allowedVariants) {
        if (variant.getRotationAngle() == rotationAngle && variant.getIsMirrored() == isMirrored) {
            return &variant;
        }
    }
    return nullptr;
}

Clipper2Lib::Paths64 PatternPiece::variantsToClipperPaths() const {
    Clipper2Lib::Paths64 paths;
    paths.reserve(m_allowedVariants.size());

    for (const auto& variant : m_allowedVariants) {
        paths.push_back(variant.getPath());
    }

    return paths;
}

} // namespace pmg::pattern