#include <utility>

#include "pattern_piece.hpp"
#include "geometry/transformations.hpp"

namespace pmg::pattern {

PatternPiece::PatternPiece(int pieceId, 
              std::vector<geometry::Point64> originalPoints, 
              const std::vector<VariantConstraint>& constraints)
    : id(pieceId), originalBoundary(std::move(originalPoints)) 
{
    allowedVariants.reserve(constraints.size());

    for (const auto& req : constraints) {
        PieceVariant variant;
        variant.rotationAngle = req.rotationAngle;
        variant.isMirrored = req.isMirrored;
        variant.requiredQuantity = req.requiredQuantity;

        variant.transformedBoundary.reserve(originalBoundary.size());
        for (const auto& point: originalBoundary) {
            auto transformedPoint = geometry::transformPoint(point, req.rotationAngle, req.isMirrored);
            variant.transformedBoundary.push_back(transformedPoint);
        }

        allowedVariants.push_back(std::move(variant));
    }
}

const PieceVariant* PatternPiece::getVariant(int rotationAngle, bool isMirrored) const noexcept {
    for (const auto& variant : allowedVariants) {
        if (variant.rotationAngle == rotationAngle && variant.isMirrored == isMirrored) {
            return &variant;
        }
    }
    return nullptr;
}

} // namespace pmg::pattern