#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "utils/timer.hpp"
#include "geometry/calculations.hpp"
#include "nesting/bottom_left.hpp"

namespace pmg::nesting {
inline constexpr int64_t SCALE = 1000;

bool BottomLeft::findPosition(NestedItem &item, std::vector<NestedItem> &items)
{
    int64_t varId = item.getVariant()->getGlobalId(item.isFlipped());

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("findPosition {} {} {} {}", item.getVariant()->getIsTwoWay(), item.isFlipped(), item.isLocked(), varId));
        for (const auto& p : item.getVariant()->getBoundary(false)) {
            SPDLOG_INFO(std::format("item point ({}, {})", p.x, p.y));
        }
    }

    // Fetch IFP for shape
    auto ifpIt = m_ifpCache.find(varId);
    if (ifpIt == m_ifpCache.end() || ifpIt->second.empty()) {
        
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO("No IFP");
        }

        return false;
    }

    Clipper2Lib::Paths64 validRegion = ifpIt->second;

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& path : validRegion) {
            SPDLOG_INFO("path in ifp");
            for (const auto& p : path) {
                SPDLOG_INFO(std::format("IFP point: ({}, {})", p.x, p.y));
            }
        }
    }

    std::vector<Clipper2Lib::Paths64> originalNFPs;
    originalNFPs.reserve(items.size());

    // Subtract NFPs of all placed items
    for (const auto& placedPiece : items) {
        if (placedPiece.isLocked()) {

            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO(std::format("Checking {} {} {} {}", placedPiece.getVariant()->getIsTwoWay(), placedPiece.isFlipped(), placedPiece.getVariant()->getGlobalId(placedPiece.isFlipped()), placedPiece.isLocked()));
            }

            geometry::NFPKey key{ placedPiece.getVariant()->getGlobalId(placedPiece.isFlipped()), varId };

            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO(std::format("key {} {}", key.variantIdA, key.variantIdB));
            }

            auto nfpIt = m_nfpCache.find(key);

            if (nfpIt != m_nfpCache.end() && !nfpIt->second.empty()) {
                
                if constexpr (config::DEBUG_MODE_DETAILED) {
                    SPDLOG_INFO("Valid nfp");
                }
                
                if constexpr (config::DEBUG_MODE_DETAILED) {
                    const Clipper2Lib::Paths64& nfpPaths = nfpIt->second;
                    for (const auto& path : nfpPaths) {
                        SPDLOG_INFO("path in nfp");
                        for (const auto& p : path) {
                            SPDLOG_INFO(std::format("NFP point: ({}, {})", p.x, p.y));
                        }
                    }
                }

                if constexpr (config::DEBUG_MODE_DETAILED) {
                    SPDLOG_INFO(std::format("position: ({}, {})", placedPiece.getPosition().x, placedPiece.getPosition().y));
                }

                Clipper2Lib::Paths64 translatedNFP = geometry::calculations::transaltePaths(nfpIt->second, geometry::calculations::toClipperPoint(placedPiece.getPosition()));

                // Create small space between NFP and IFP for cases of 1D degenerated lines being valid positions
                originalNFPs.push_back(translatedNFP);
                translatedNFP = Clipper2Lib::InflatePaths(
                    translatedNFP,
                    -1.0,
                    Clipper2Lib::JoinType::Miter,
                    Clipper2Lib::EndType::Polygon
                );

                if constexpr (config::DEBUG_MODE_DETAILED) {
                    for (const auto& path : translatedNFP) {
                        SPDLOG_INFO("path in translated nfp");
                        for (const auto& p : path) {
                            SPDLOG_INFO(std::format("NFP point: ({}, {})", p.x, p.y));
                        }
                    }
                }
                
                validRegion = Clipper2Lib::Difference(
                    validRegion,
                    translatedNFP,
                    Clipper2Lib::FillRule::NonZero
                );

                if (validRegion.empty()) {
                    
                    if constexpr (config::DEBUG_MODE_DETAILED) {
                        SPDLOG_INFO("No valid region");
                    }

                    return false;
                }
            }
        } else {
            break;
        }
    }

    validRegion = Clipper2Lib::SimplifyPaths(validRegion, 0.0);

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& path : validRegion) {
            SPDLOG_INFO("path in valid region");
            for (const auto& p : path) {
                SPDLOG_INFO(std::format("valid region point: ({}, {})", p.x, p.y));
            }
        }
    }

    // Extract vertices
    std::vector<Clipper2Lib::Point64> candidatePoints;
    for (const auto& path : validRegion) {
        for (const auto& point : path) {
            candidatePoints.push_back(point);
        }
    }

    if (candidatePoints.empty()) {
        return false;
    }

    // Sort first by x and then by y
    std::sort(candidatePoints.begin(), candidatePoints.end(),  
        [](const Clipper2Lib::Point64& a, const Clipper2Lib::Point64& b) {
            if (a.x != b.x) return a.x < b.x;
            return a.y < b.y;
        });

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& p : candidatePoints) {
            SPDLOG_INFO(std::format("candidate point: ({}, {})", p.x, p.y));
        }
    }

    // Lock item into place
    Clipper2Lib::Point64 bestPosition = candidatePoints.front();

    // Snap to boundaries so the inflated NFP won't affect coordinates values
    bestPosition = geometry::calculations::snapToBoundaries(bestPosition, ifpIt->second, originalNFPs);

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("best position: ({}, {})", bestPosition.x, bestPosition.y));
    }

    m_placedPiecesPolygons.push_back(item.getItemPathAtPosition(bestPosition.x, bestPosition.y));
    m_placedPiecesBoxes.push_back(item.getItemBoundingBoxAtPosition(bestPosition.x, bestPosition.y));
    item.lockInPosition(bestPosition.x, bestPosition.y);

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO("Item locked");
        SPDLOG_INFO(item.toString());
    }

    return true;
}

BottomLeft::BottomLeft(pattern::Fabric fabric, std::unordered_map<int64_t, Clipper2Lib::Paths64>& ifpCache, std::unordered_map<geometry::NFPKey, Clipper2Lib::Paths64>& nfpCache)
    : m_fabric(std::move(fabric)), m_ifpCache(ifpCache), m_nfpCache(nfpCache) {}

NestingResult BottomLeft::performNesting(std::vector<NestedItem> &items)
{
    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO("Starting nesting");
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& point : m_fabric.getBoundary()) {
            SPDLOG_INFO(std::format("Fabric point {}", point.toString()));
        }
    }

    NestingResult result;

    int64_t minLen = 0;

    for (size_t i = 0; i < items.size(); ++i) {
        NestedItem& item = items[i];
        
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("Nesting item no. {} {} {}", i, item.getVariant()->getIsTwoWay(), item.isFlipped()));
        }

        if (!findPosition(item, items)) {
            
            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO("Could not find position");
            }

            return { .wasSuccessful = false };
        }

        if (item.getItemBoundingBoxAtPosition().right > minLen) {
            minLen = item.getItemBoundingBoxAtPosition().right;
        }

        geometry::Point64 rawPosition = item.getPosition();

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("Raw position point {} for {}", rawPosition.toString(), item.getVariant()->getPieceId()));
        }

        geometry::Point64 positionWithoutAllowances = item.getPositionForItemWithoutAllowances();
        geometry::Point64 refPoint = item.getVariant()->getReferencePoint();

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("Without allowances position point {}", positionWithoutAllowances.toString()));
            SPDLOG_INFO(std::format("xmax - xt = {} - {}", m_fabric.getBoundingBox().right, positionWithoutAllowances.x));
        }

        geometry::Point64 deltaPosition(positionWithoutAllowances.y, m_fabric.getBoundingBox().right - positionWithoutAllowances.x);

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("World position point {}", deltaPosition.toString()));
        }

        deltaPosition.x -= refPoint.x;
        deltaPosition.y -= refPoint.y;

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("Delta point {}", deltaPosition.toString()));
        }

        result.positions.push_back({ 
            .id = item.getVariant()->getPieceId(), 
            .rotationAngle = item.getVariant()->getRotationAngle(),
            .isMirrored = item.getVariant()->getIsMirrored(),
            .isFlipped180 = item.isFlipped(),
            .x = deltaPosition.x / SCALE,
            .y = deltaPosition.y / SCALE
        });
    }

    result.wasSuccessful = true;
    result.minLength = static_cast<double>(minLen) / (SCALE * SCALE);
    result.stats = geometry::calculations::evaluate(m_placedPiecesPolygons, m_fabric, minLen);

    return result;
}

} // namespace pmg::nesting
