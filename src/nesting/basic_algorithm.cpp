#include <spdlog/spdlog.h>

#include <algorithm>

#include "utils/config.hpp"
#include "geometry/svg_exporter.hpp"
#include "nesting/basic_algorithm.hpp"

namespace pmg::nesting {

NestingResult BasicAlgorithm::runAlgorithm()
{
    std::vector<NestedItem> sortedItems = m_originalItems;

    std::ranges::sort(sortedItems, [](const NestedItem&a, const NestedItem& b) {
        auto aBounds = a.getItemBoundingBoxAtPosition();
        auto bBounds = b.getItemBoundingBoxAtPosition();
        return (aBounds.bottom - aBounds.top) > (bBounds.bottom - bBounds.top);
    });

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO("Items in queue");
        for (const auto& item : sortedItems) {
            SPDLOG_INFO(item.toString());
        }
        SPDLOG_INFO("End of queue");
    }

    NestingResult result;

    if constexpr (config::DEVELOPMENT) SPDLOG_INFO("Running single Bottom Left Fill...");
    
    result = runSingleBottomLeft(sortedItems);

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO("Items after single BLF");
        for (const auto& item : sortedItems) {
            SPDLOG_INFO(item.toString());
        }
        SPDLOG_INFO("End of items");
    }

    if constexpr (config::DEVELOPMENT) {
        pmg::geometry::SvgExporter::exportLayout(
            "debug_output.svg", 
            m_fabric,
            sortedItems,
            sortedItems[0].getVariant()->getSeamAllowance(),
            sortedItems[0].getVariant()->getCuttingAllowance()
        );
    }

    return result;
}

} // namespace pmg::nesting