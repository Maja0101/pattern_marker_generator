#include <spdlog/spdlog.h>

#include "nesting/bottom_left.hpp"
#include "geometry/calculations.hpp"
#include "utils/config.hpp"
#include "utils/timer.hpp"

namespace pmg::nesting {
BLStateView BottomLeft::makeCurrStateView() const
{
    return BLStateView{ m_fabric, m_placed_pieces_polygons, m_placed_pieces_boxes };
}

bool BottomLeft::findPosition(NestedItem &item, std::vector<NestedItem> &items)
{
    // timing::Timer timer;

    int64_t varId = item.getVariant()->getGlobalId();

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(varId);
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
                SPDLOG_INFO(std::format("IFP: ({}, {})", p.x, p.y));
            }
        }
    }

    std::vector<Clipper2Lib::Paths64> originalNFPs;
    originalNFPs.reserve(items.size());

    // Subtract NFPs of all placed items
    for (const auto& placedPiece : items) {
        if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO(std::format("Checking {} {}", placedPiece.getVariant()->getGlobalId(), placedPiece.isLocked()));
        }
        if (placedPiece.isLocked()) {
            if constexpr (config::DEBUG_MODE) {
                SPDLOG_INFO("Processing");
            }
            geometry::NFPKey key{ placedPiece.getVariant()->getGlobalId(), varId };

            auto nfpIt = m_nfpCache.find(key);

            if (nfpIt != m_nfpCache.end() && !nfpIt->second.empty()) {
                if constexpr (config::DEBUG_MODE) {
                    SPDLOG_INFO("Valid nfp");
                }
                
                if constexpr (config::DEBUG_MODE) {
                    const Clipper2Lib::Paths64& nfpPaths = nfpIt->second;
                    for (const auto& path : nfpPaths) {
                        SPDLOG_INFO("path in nfp");
                        for (const auto& p : path) {
                            SPDLOG_INFO(std::format("NFP: ({}, {})", p.x, p.y));
                        }
                    }
                }

                if constexpr (config::DEBUG_MODE) {
                    SPDLOG_INFO(std::format("position: ({}, {})", placedPiece.getPosition().x, placedPiece.getPosition().y));
                }

                Clipper2Lib::Paths64 translatedNFP = geometry::calculations::transaltePaths(nfpIt->second, geometry::calculations::toClipperPoint(placedPiece.getPosition()));
                originalNFPs.push_back(translatedNFP);
                translatedNFP = Clipper2Lib::InflatePaths(
                    translatedNFP,
                    -1.0,
                    Clipper2Lib::JoinType::Miter,
                    Clipper2Lib::EndType::Polygon
                );

                if constexpr (config::DEBUG_MODE) {
                    for (const auto& path : translatedNFP) {
                        SPDLOG_INFO("path in translated nfp");
                        for (const auto& p : path) {
                            SPDLOG_INFO(std::format("NFP: ({}, {})", p.x, p.y));
                        }
                    }
                }
                
                validRegion = Clipper2Lib::Difference(
                    validRegion,
                    translatedNFP,
                    Clipper2Lib::FillRule::NonZero
                );

                if (validRegion.empty()) {
                    if constexpr (config::DEBUG_MODE) {
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

    if constexpr (config::DEBUG_MODE) {
        for (const auto& path : validRegion) {
            SPDLOG_INFO("path in valid region");
            for (const auto& p : path) {
                SPDLOG_INFO(std::format("validRegion: ({}, {})", p.x, p.y));
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

    if constexpr (config::DEBUG_MODE) {
        for (const auto& p : candidatePoints) {
            SPDLOG_INFO(std::format("candidatePoints: ({}, {})", p.x, p.y));
        }
    }

    // Lock item into place
    Clipper2Lib::Point64 bestPosition = candidatePoints.front();

    bestPosition = geometry::calculations::snapToBoundaries(bestPosition, ifpIt->second, originalNFPs);

    if constexpr (config::DEBUG_MODE) {
        SPDLOG_INFO(std::format("bestPosition: ({}, {})", bestPosition.x, bestPosition.y));
    }

    m_placed_pieces_polygons.push_back(item.getItemPathAtPosition(bestPosition.x, bestPosition.y));
    m_placed_pieces_boxes.push_back(item.getItemBoundingBoxAtPosition(bestPosition.x, bestPosition.y));
    item.lockInPosition(bestPosition.x, bestPosition.y);

    if constexpr (config::DEBUG_MODE) {
        SPDLOG_INFO("Item locked");
        SPDLOG_INFO(item.toString());
    }

    return true;
}

BottomLeft::BottomLeft(pattern::Fabric fabric, std::unordered_map<int64_t, Clipper2Lib::Paths64>& ifpCache, std::unordered_map<geometry::NFPKey, Clipper2Lib::Paths64>& nfpCache)
    : m_fabric(std::move(fabric)), m_ifpCache(ifpCache), m_nfpCache(nfpCache) {}

bool BottomLeft::performNesting(std::vector<NestedItem> &items)
{
    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO("Starting nesting");
    }

    int i = 0;
    for (auto& item : items) {
        ++i;
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("Nesting item no. {}", i));
        }
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("Nesting item no. {}", i));
            SPDLOG_INFO(std::format("Nesting for {}", item.toString()));
        }

        if (!findPosition(item, items)) {
            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO("Could not find position");
            }
            return false;
        }
    }

    return true;
}

} // namespace pmg::nesting
