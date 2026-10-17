#include <utility>
#include <format>
#include <string>

#include "pattern/pattern_piece.hpp"
#include "geometry/transformations.hpp"
#include "geometry/calculations.hpp"

namespace pmg::pattern {
PatternPiece::PatternPiece(int64_t pieceId, 
        std::vector<geometry::Point64> originalPoints, 
        const std::vector<VariantConstraint>& constraints,
        int64_t seamAllowance, int64_t cuttingAllowance)
    : m_id(pieceId), m_originalPolygon(std::move(originalPoints)) 
{
    m_allowedVariants.reserve(constraints.size());

    std::vector<geometry::Point64> offsetPath = 
        geometry::calculations::calculatePolygonWithAllowance(
            m_originalPolygon, 
            seamAllowance, 
            cuttingAllowance
    );

    int variantId = 0;
    for (const auto& req : constraints) {
        if (req.requiredQuantity > 0) {
            std::vector<pmg::geometry::Point64> transformedBoundary;
            transformedBoundary.reserve(offsetPath.size());

            transformedBoundary = geometry::transformPolygon(offsetPath, req.rotationAngle, req.isMirrored);

            PieceVariant variant(++variantId, req.rotationAngle, req.isMirrored, req.requiredQuantity, transformedBoundary);

            m_allowedVariants.push_back(std::move(variant));
        }
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

std::string PatternPiece::toString() const
{
    std::string boundaryStr = std::accumulate(
        std::next(m_originalPolygon.begin()), m_originalPolygon.end(),
        m_originalPolygon.front().toString(),
        [](std::string a, geometry::Point64 b) {
            return a + ", " + b.toString();
        }
    );

    std::string variantsStr = std::accumulate(
        std::next(m_allowedVariants.begin()), m_allowedVariants.end(),
        m_allowedVariants.front().toString(),
        [](std::string a, pattern::PieceVariant b) {
            return a + "; " + b.toString();
        }
    );

    return std::format("Pattern piece no. {} [{}] with variants: {}",
                            m_id,
                            boundaryStr,
                            variantsStr);
}

} // namespace pmg::pattern