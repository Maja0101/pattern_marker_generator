#include <spdlog/spdlog.h>

#include <algorithm>

#include "utils/config.hpp"
#include "geometry/svg_exporter.hpp"
#include "nesting/basic_algorithm.hpp"

namespace pmg::nesting {

NestingResult BasicAlgorithm::runAlgorithm()
{
    // std::vector<NestedItem> sortedItems = m_originalItems;

    // std::ranges::sort(m_originalItems, [](const NestedItem&a, const NestedItem& b) {
    //     auto aBounds = a.getItemBoundingBoxAtPosition();
    //     auto bBounds = b.getItemBoundingBoxAtPosition();
    //     return (aBounds.bottom - aBounds.top) > (bBounds.bottom - bBounds.top);
    // });

    std::ranges::stable_sort(m_originalItems, [](const NestedItem& a, const NestedItem& b) {
        auto aBounds = a.getItemBoundingBoxAtPosition();
        auto bBounds = b.getItemBoundingBoxAtPosition();
        int64_t aHeight = aBounds.bottom - aBounds.top;
        int64_t bHeight = bBounds.bottom - bBounds.top;

        if (aHeight != bHeight) {
            return aHeight > bHeight;
        }

        const auto* aVar = a.getVariant();
        const auto* bVar = b.getVariant();
        if (aVar->getPieceId() != bVar->getPieceId()) {
            return aVar->getPieceId() < bVar->getPieceId();
        }

        return aVar->getIsMirrored() < bVar->getIsMirrored();
    });

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO("Items in queue");
        for (const auto& item : m_originalItems) {
            SPDLOG_INFO(item.toString());
        }
        SPDLOG_INFO("End of queue");
    }

    NestingResult result;

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
        pmg::geometry::SvgExporter::exportLayout(
            "debug_output.svg", 
            m_fabric,
            m_originalItems,
            m_originalItems[0].getVariant()->getSeamAllowance(),
            m_originalItems[0].getVariant()->getCuttingAllowance()
        );
    }

    return result;
}

} // namespace pmg::nesting