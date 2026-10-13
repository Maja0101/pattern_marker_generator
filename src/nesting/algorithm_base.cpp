#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "nesting/algorithm_base.hpp"
#include "nesting/bottom_left.hpp"
#include "nesting/calculations.hpp"

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

    m_originalItems = calculations::prepareItems(patternPieces);

    if constexpr (config::DEBUG_MODE) {
        for (const auto& item : m_originalItems) {
            SPDLOG_INFO(item.toString());
        }
    }

    runAlgorithm();

    return false;
}

} // namespace pmg::nesting