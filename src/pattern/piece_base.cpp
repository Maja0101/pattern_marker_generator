#include "piece_base.hpp"
#include <utility>

namespace pmg::pattern {

PieceBase::PieceBase(int pieceId, 
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

        variant.transformedBoundary.reserve(originalPoints.size());
        // transform points and push into boundary vector

        allowedVariants.push_back(std::move(variant));
    }
}

const PieceVariant* PieceBase::getVariant(int rotationAngle, bool isMirrored) const noexcept {
    for (const auto& variant : allowedVariants) {
        if (variant.rotationAngle == rotationAngle && variant.isMirrored == isMirrored) {
            return &variant;
        }
    }
    return nullptr;
}

} // namespace pmg::pattern