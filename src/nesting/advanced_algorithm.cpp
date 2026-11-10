#include <algorithm>

#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "geometry/svg_exporter.hpp"
#include "geometry/options_calculations.hpp"
#include "nesting/advanced_algorithm.hpp"

namespace pmg::nesting {

void AdvancedAlgorithm::evaluateDifferentRotations(std::vector<nesting::NestedItem> &items, size_t index, NestingScore &best)
{
    if (index == items.size()) {
        NestingResult currentResult = runSingleBottomLeft(items);
        double currentScore = currentResult.wasSuccessful ? geometry::calculations::calculateNestingScore(currentResult.stats) : -1.0;

        if (currentScore > best.score) {

            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO(std::format("curr best {} score {} result {} {} {}, {} {} {} {} {}, {} {}", best.score, currentScore, 
                    currentResult.stats.remainingFabricLength,
                    currentResult.stats.remainingFabricArea,
                    currentResult.stats.remainingFabricPercentage,
                    currentResult.stats.usableScrapCount,
                    currentResult.stats.usableScrapsArea,
                    currentResult.stats.usableScrapsPercentage,
                    currentResult.stats.effectiveScrapsArea,
                    currentResult.stats.effectiveScrapsPercentage,
                    currentResult.stats.wasteArea,
                    currentResult.stats.wastePercentage
                ));
            }

            best.result = std::move(currentResult);
            best.score = std::move(currentScore);

            if constexpr (config::DEVELOPMENT) {
                pmg::geometry::SvgExporter::exportLayout(
                    std::format("debug_output_{}.svg", ++m_svgExport), 
                    m_fabric,
                    m_originalItems,
                    m_originalItems[0].getVariant()->getSeamAllowance(),
                    m_originalItems[0].getVariant()->getCuttingAllowance()
                );
            }
        }

        for (auto& item : items) {
            item.resetPosition();
        }

        return;
    }

    evaluateDifferentRotations(items, index + 1, best);

    if (items[index].getVariant()->getIsTwoWay()) {
        items[index].flip180();
        evaluateDifferentRotations(items, index + 1, best);
        items[index].flip180();
    }
}

NestingResult AdvancedAlgorithm::runAlgorithm()
{
    std::ranges::sort(m_originalItems, [](const NestedItem&a, const NestedItem& b) {
        return a.getVariant()->getGlobalId(false) < b.getVariant()->getGlobalId(false);
    });

    NestingScore best;
    best.score = -1.0;

    if(geometry::calculations::numberOfOptionsWithinLimit(m_originalItems)) {
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO("Checking all options");
        }
        m_svgExport = 0;

        do {
            evaluateDifferentRotations(m_originalItems, 0, best);
        } while (std::ranges::next_permutation(m_originalItems, [](const NestedItem& a, const NestedItem&b) {
            return a.getVariant()->getGlobalId(false) < b.getVariant()->getGlobalId(false);
        }).found);

        if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO("Last Svg {}", m_svgExport);
        }

    } else {

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO("Checking one option");
        }

        int64_t currIdx = m_originalItems[0].getVariant()->getGlobalId(false);
        size_t currIdxCount = 0;

        for (auto& item : m_originalItems) {
            if (item.getVariant()->getGlobalId(false) != currIdx) {
                currIdx = item.getVariant()->getGlobalId(false);
                currIdxCount = 0;
            }
                
            if (currIdxCount % 2 != 0) {
                item.flip180();
            }

            ++currIdxCount;
        }

        std::ranges::sort(m_originalItems, [](const NestedItem&a, const NestedItem& b) {
            auto aBounds = a.getItemBoundingBoxAtPosition();
            auto bBounds = b.getItemBoundingBoxAtPosition();
            return (aBounds.bottom - aBounds.top) > (bBounds.bottom - bBounds.top);
        });

        best.result = std::move(runSingleBottomLeft(m_originalItems));

        if constexpr (config::DEVELOPMENT) {
            pmg::geometry::SvgExporter::exportLayout(
                "debug_output.svg", 
                m_fabric,
                m_originalItems,
                m_originalItems[0].getVariant()->getSeamAllowance(),
                m_originalItems[0].getVariant()->getCuttingAllowance()
            );
        }
    }

    return best.result;
}

} // namespace pmg::nesting