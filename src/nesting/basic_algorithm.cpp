#include <spdlog/spdlog.h>

#include <algorithm>

#include "utils/config.hpp"
#include "utils/timer.hpp"
#include "pattern/svg_exporter.hpp"
#include "geometry_calculations/nfp_ifp_calculations.hpp"
#include "geometry_calculations/calculations.hpp"
#include "nesting/basic_algorithm.hpp"

namespace pmg::nesting {
void BasicAlgorithm::sortItems()
{
    std::ranges::stable_sort(m_originalItems, [](const pattern::NestedItem& a, const pattern::NestedItem& b) {
        auto aBounds = a.getItemBoundingBoxAtPosition();
        auto bBounds = b.getItemBoundingBoxAtPosition();

        int64_t aHeight = aBounds.bottom - aBounds.top;
        int64_t bHeight = bBounds.bottom - bBounds.top;

        if (aHeight != bHeight) {
            return aHeight > bHeight;
        }

        const auto* aVariant = a.getVariant();
        const auto* bVariant = b.getVariant();

        if (aVariant->getPieceId() != bVariant->getPieceId()) {
            return aVariant->getPieceId() < bVariant->getPieceId();
        }

        return !aVariant->getIsMirrored() && bVariant->getIsMirrored();
    });
}

pattern::NestingResult BasicAlgorithm::runAlgorithm()
{
    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO("Items in queue");
        for (const auto& item : m_originalItems) {
            SPDLOG_INFO(item.toString());
        }
        SPDLOG_INFO("End of queue");
    }

    pattern::NestingResult result;

    if constexpr (config::DEVELOPMENT) SPDLOG_INFO("Running single Bottom Left Fill...");
    
    result = runSingleBottomLeft(m_originalItems);

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO("Items after single BLF");
        for (const auto& item : m_originalItems) {
            SPDLOG_INFO(item.toString());
        }
        SPDLOG_INFO("End of items");
    }

    if constexpr (config::DEVELOPMENT) {
        visualisation::SvgExporter::exportLayout(
            "debug_output.svg", 
            m_fabric,
            m_originalItems,
            m_originalItems[0].getVariant()->getSeamAllowance(),
            m_originalItems[0].getVariant()->getCuttingAllowance()
        );
    }

    return result;
}

void BasicAlgorithm::prepareNestingItems(std::vector<pattern::PatternPiece> &patternPieces)
{
    if constexpr (config::DEVELOPMENT) SPDLOG_INFO("Preparing the geometry...");
        
    timing::Timer timer;

    for (auto& piece : patternPieces) {
        for (auto& variant : piece.getVariants()) {
            variant.setIsTwoWay(false);
        }
    }

    m_originalItems = geometry::calculations::prepareItems(patternPieces);
    sortItems();

    std::vector<pmg::pattern::PieceVariant> allPossibleVariants = geometry::calculations::getAllVariants(patternPieces);

    m_ifpCache = geometry::calculations::calculateIFPCache(m_fabric, allPossibleVariants);

    m_nfpCache = geometry::calculations::calculateNFPCacheForFixedQueue(m_originalItems, allPossibleVariants.size());

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_WARN(std::format("variants: {} items: {}", allPossibleVariants.size(), m_originalItems.size()));
        SPDLOG_INFO(std::format("ifp cache: {}, nfp cache: {}", m_ifpCache.size(), m_nfpCache.size()));
        for (const auto& [key, val] : m_ifpCache) {
            SPDLOG_INFO(std::format("ifp cache id {}", key));
        }
        for (const auto& [key, val] : m_nfpCache) {
            SPDLOG_INFO(std::format("nfp cache id {} {}", key.variantIdA, key.variantIdB));
        }
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& item : m_originalItems) {
            SPDLOG_INFO(item.toString());
        }
    }
}

} // namespace pmg::nesting