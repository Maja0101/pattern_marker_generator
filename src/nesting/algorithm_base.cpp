#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "nesting/algorithm_base.hpp"
#include "nesting/bottom_left.hpp"
#include "geometry/calculations.hpp"

namespace pmg::nesting {

bool AlgorithmBase::runSingleBottomLeft(std::vector<nesting::NestedItem> &items)
{
    // TODO should return the layout and the statistics data

    BottomLeft bottomLeft(m_fabric);

    return bottomLeft.performNesting(items);
}

AlgorithmBase::AlgorithmBase(pattern::Fabric fabric)
    : m_fabric(std::move(fabric)) {}

bool AlgorithmBase::execute(const std::vector<pattern::PatternPiece> &patternPieces)
{
    // TODO should return the layout and the statistics data

    m_originalItems = geometry::calculations::prepareItems(patternPieces);

    // TODO based on variants in pattern pieces prepare cache for nfp and ifp

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& item : m_originalItems) {
            SPDLOG_INFO(item.toString());
        }
    }

    runAlgorithm();

    return false;
}

} // namespace pmg::nesting