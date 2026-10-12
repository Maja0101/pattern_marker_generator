#include <spdlog/spdlog.h>

#include "nesting/basic_algorithm.hpp"
#include "geometry/svg_exporter.hpp"

namespace pmg::nesting {

bool BasicAlgorithm::runAlgorithm()
{
    std::vector<NestedItem> sorted_items = m_originalItems;

    // std::sort(sorted_items.begin(), sorted_items.end(), [](const NestedItem&a, const NestedItem& b) {
    //     auto a_bounds = a.getItemBoundingBoxAtPosition();
    //     auto b_bounds = b.getItemBoundingBoxAtPosition();
    //     return (a_bounds.bottom - a_bounds.top) > (b_bounds.bottom - b_bounds.top);
    // });

    bool result = runSingleBottomLeft(sorted_items);

    SPDLOG_WARN(((m_fabric.getBoundingBox().right - m_fabric.getBoundingBox().left) / 1000));
    SPDLOG_WARN(((m_fabric.getBoundingBox().bottom - m_fabric.getBoundingBox().top) / 1000));

    bool exportResult = pmg::geometry::SvgExporter::exportLayout(
        "debug_output.svg", 
        (m_fabric.getBoundingBox().right - m_fabric.getBoundingBox().left) / 1000, 
        (m_fabric.getBoundingBox().bottom - m_fabric.getBoundingBox().top) / 1000, 
        sorted_items
    );

    return result;
}

} // namespace pmg::nesting