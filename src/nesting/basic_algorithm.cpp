#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "utils/timer.hpp"
#include "nesting/basic_algorithm.hpp"
#include "geometry/svg_exporter.hpp"

namespace pmg::nesting {

NestingResult BasicAlgorithm::runAlgorithm()
{
    std::vector<NestedItem> sortedItems = m_originalItems;


    std::sort(sortedItems.begin(), sortedItems.end(), [](const NestedItem&a, const NestedItem& b) {
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

    {
        SPDLOG_INFO("Running single Bottom Left Fill...");
        timing::Timer timer;
        result = runSingleBottomLeft(sortedItems);
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO("Items after single BL");
        for (const auto& item : sortedItems) {
            SPDLOG_INFO(item.toString());
        }
        SPDLOG_INFO("End of items");
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("Svg generation: w={}, h={}", 
            ((m_fabric.getBoundingBox().Width()) / 1000), 
            (m_fabric.getBoundingBox().Height()) / 1000)
        );
    }

    bool exportResult = pmg::geometry::SvgExporter::exportLayout(
        "debug_output.svg", 
        m_fabric,
        sortedItems,
        sortedItems[0].getVariant()->getSeamAllowance(),
        sortedItems[0].getVariant()->getCuttingAllowance()
    );

    return result;
}

} // namespace pmg::nesting