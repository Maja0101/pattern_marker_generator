#include <utility>
#include <format>
#include <string>

#include "pattern/pattern_piece.hpp"
#include "geometry/transformations.hpp"

namespace pmg::pattern {

PatternPiece::PatternPiece(int64_t pieceId, 
              std::vector<geometry::Point64> originalPoints, 
              const std::vector<VariantConstraint>& constraints)
    : m_id(pieceId), m_originalBoundary(std::move(originalPoints)) 
{
    m_allowedVariants.reserve(constraints.size());

    // TODO the transformation of the original boundary into all possible variants should be updated 
    for (const auto& req : constraints) {
        std::vector<pmg::geometry::Point64> transformedBoundary;
        transformedBoundary.reserve(m_originalBoundary.size());

        transformedBoundary = geometry::transformPolygon(m_originalBoundary, req.rotationAngle, req.isMirrored);

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

std::string PatternPiece::toString() const
{
    std::string boundaryStr = std::accumulate(
        std::next(m_originalBoundary.begin()), m_originalBoundary.end(),
        m_originalBoundary.front().toString(),
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