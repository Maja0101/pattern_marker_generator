#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "nesting/algorithm_base.hpp"
#include "nesting/bottom_left.hpp"

namespace pmg::nesting {

std::vector<nesting::NestedItem> AlgorithmBase::prepareItems(const std::vector<pattern::PatternPiece> &patternPieces)
{
    std::vector<nesting::NestedItem> items;

    for (const auto& piece : patternPieces) {
        if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO(piece.toString());
        }
        for (const auto& variant : piece.getVariants()) {
            if constexpr (config::DEBUG_MODE) {
                SPDLOG_INFO(variant.toString());
            }
            for (int i = 0; i < variant.getRequiredQuantity(); ++i) {
                if constexpr (config::DEBUG_MODE) {
                    SPDLOG_INFO(std::format("Loop no. {}", i+1));
                }

                items.emplace_back(&variant);
            }
        }
    }

    if constexpr (config::DEBUG_MODE) {
        for (const auto& item : items) {
            SPDLOG_INFO(item.toString());
        }
    }
    
    return items;
}

bool AlgorithmBase::runSingleBottomLeft(std::vector<nesting::NestedItem> &items)
{
    // TODO should return the layout and the statistics data

    BottomLeft bottomLeft(m_fabric);

    return bottomLeft.performNesting(items);
}

bool AlgorithmBase::evaluate(const std::vector<nesting::NestedItem> &items)
{
    // TODO evaluation function for layout 

    return false;
}

AlgorithmBase::AlgorithmBase(pattern::Fabric fabric)
    : m_fabric(std::move(fabric)) {}

bool AlgorithmBase::execute(const std::vector<pattern::PatternPiece> &patternPieces)
{
    // TODO should return the layout and the statistics data

    m_originalItems = prepareItems(patternPieces);

    if constexpr (config::DEBUG_MODE) {
        for (const auto& item : m_originalItems) {
            SPDLOG_INFO(item.toString());
        }
    }

    runAlgorithm();

    return false;
}

} // namespace pmg::nesting