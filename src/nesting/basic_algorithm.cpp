#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "utils/timer.hpp"
#include "nesting/basic_algorithm.hpp"
#include "geometry/svg_exporter.hpp"

namespace pmg::nesting {

bool BasicAlgorithm::runAlgorithm()
{
    std::vector<NestedItem> sorted_items = m_originalItems;

    std::sort(sorted_items.begin(), sorted_items.end(), [](const NestedItem&a, const NestedItem& b) {
        auto a_bounds = a.getItemBoundingBoxAtPosition();
        auto b_bounds = b.getItemBoundingBoxAtPosition();
        return (a_bounds.bottom - a_bounds.top) > (b_bounds.bottom - b_bounds.top);
    });

    if constexpr (config::DEBUG_MODE) {
        SPDLOG_INFO("Items in queue");
        for (const auto& item : sorted_items) {
            SPDLOG_INFO(item.toString());
        }
        SPDLOG_INFO("End of queue");
    }

    bool result;

    {
        timing::Timer timer;

        result = runSingleBottomLeft(sorted_items);
    }

    if constexpr (config::DEBUG_MODE) {
        SPDLOG_INFO("Items after single BL");
        for (const auto& item : sorted_items) {
            SPDLOG_INFO(item.toString());
        }
        SPDLOG_INFO("End of items");
    }

    if constexpr (config::DEBUG_MODE) {
        SPDLOG_INFO(std::format("Svg generation: w={}, h={}", 
            ((m_fabric.getBoundingBox().Width()) / 1000), 
            (m_fabric.getBoundingBox().Height()) / 1000)
        );
    }

    {
        timing::Timer timer;

        bool exportResult = pmg::geometry::SvgExporter::exportLayout(
            "debug_output.svg", 
            (m_fabric.getBoundingBox().Width()) / 1000, 
            (m_fabric.getBoundingBox().Height()) / 1000, 
            sorted_items
        );
    }

    return result;
}

} // namespace pmg::nesting