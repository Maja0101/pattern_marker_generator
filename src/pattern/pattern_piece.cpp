#include <utility>
#include <format>
#include <string>

#include <clipper2/clipper.h>
#include <spdlog/spdlog.h>

#include "geometry/transformations.hpp"
#include "geometry/calculations.hpp"
#include "pattern/pattern_piece.hpp"
#include "utils/config.hpp"

namespace pmg::pattern {
inline constexpr int64_t SCALE = 1000;

void PatternPiece::initializePatternPieceGeometry(const std::vector<VariantConstraint> &constraints, bool toBeScaled)
{
    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& p : m_originalPolygon) {
            SPDLOG_INFO(std::format("original polygon point {}", p.toString()));
        }
    }

    std::vector<geometry::Point64> simplifiedPolygon = geometry::calculations::fromClipperPath(Clipper2Lib::SimplifyPath(geometry::calculations::toClipperPath(m_originalPolygon), 0.1));

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& p : simplifiedPolygon) {
            SPDLOG_INFO(std::format("simplified polygon point {}", p.toString()));
        }
    }

    if (toBeScaled) {
        for (auto& point : simplifiedPolygon) {
            point.x *= SCALE;
            point.y *= SCALE;
        }
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& p : simplifiedPolygon) {
            SPDLOG_INFO(std::format("scaled polygon point {}", p.toString()));
        }
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("sum allowances {}", static_cast<double>(m_seamAllowance + m_cuttingAllowance)));
    }

    std::vector<geometry::Point64> offsetPath = 
        geometry::calculations::calculatePolygonWithAllowance(
            simplifiedPolygon, 
            m_seamAllowance, 
            m_cuttingAllowance
    );

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& p : offsetPath) {
            SPDLOG_INFO(std::format("offset polygon point {}", p.toString()));
        }
    }

    m_allowedVariants.reserve(constraints.size());

    int variantId = 0;
    for (const auto& req : constraints) {
        if (req.requiredQuantity > 0) {
            ++variantId;

            std::vector<pmg::geometry::Point64> transformedBoundary;
            transformedBoundary.reserve(offsetPath.size());

            transformedBoundary = geometry::calculations::transformPolygon(offsetPath, m_grainline + req.rotationAngle, req.isMirrored, false);

            if constexpr (config::DEBUG_MODE_DETAILED) {
                for (const auto& p : transformedBoundary) {
                    SPDLOG_INFO(std::format("rotated by alpha polygon point {}", p.toString()));
                }
            }

            geometry::Point64 referencePoint = geometry::calculations::getTopLeftCorner(transformedBoundary);

            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO(std::format("reference point {}", referencePoint.toString()));
            }

            // Rotate shape by 90 degrees ccw to match the algorithm logic
            transformedBoundary = geometry::calculations::transformPolygon(transformedBoundary, 270, false, true);

            if constexpr (config::DEBUG_MODE_DETAILED) {
                for (const auto& p : transformedBoundary) {
                    SPDLOG_INFO(std::format("rotated by 90 ccw polygon point {}", p.toString()));
                }
            }

            PieceVariant variant(variantId, m_id, req.rotationAngle, req.isMirrored, req.requiredQuantity, req.twoWay, m_seamAllowance, m_cuttingAllowance, transformedBoundary, referencePoint);

            m_allowedVariants.push_back(std::move(variant));
        }
    }
}

PatternPiece::PatternPiece(int64_t pieceId, 
        std::vector<geometry::Point64> originalPoints, 
        const std::vector<VariantConstraint>& constraints,
        int64_t seamAllowance, int64_t cuttingAllowance, 
        int grainline, bool toBeScaled)
    : m_id(pieceId), m_originalPolygon(std::move(originalPoints)) , m_seamAllowance(toBeScaled ? seamAllowance * SCALE : seamAllowance), m_cuttingAllowance(toBeScaled ? cuttingAllowance * SCALE : cuttingAllowance), m_grainline(grainline)
{
    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("seam {} cutting {} allowance", m_seamAllowance, m_cuttingAllowance));
    }

    initializePatternPieceGeometry(constraints, toBeScaled);
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