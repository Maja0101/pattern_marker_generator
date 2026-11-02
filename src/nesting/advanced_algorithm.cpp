#include <algorithm>
#include <unordered_map>

#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "utils/timer.hpp"
#include "geometry/svg_exporter.hpp"
#include "nesting/advanced_algorithm.hpp"
#include "geometry/options_calculations.hpp"

namespace pmg::nesting {

void AdvancedAlgorithm::evaluateDifferentRotations(std::vector<nesting::NestedItem> &items, size_t index, NestingScore &best)
{
    if (index == items.size()) {
        NestingResult currentResult = runSingleBottomLeft(items);
        double currentScore = currentResult.wasSuccessful ? geometry::calculations::calculateNestingScore(currentResult.stats) : -1.0;

        if (currentScore > best.score) {
            best.result = std::move(currentResult);
            best.score = std::move(currentScore);
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
    std::sort(m_originalItems.begin(), m_originalItems.end(), [](const NestedItem&a, const NestedItem& b) {
        return a.getVariant()->getGlobalId(false) < b.getVariant()->getGlobalId(false);
    });

    NestingScore best;

    if(geometry::calculations::numberOfOptionsWithinLimit(m_originalItems)) {
        double bestScore = -1.0;

        do {
            evaluateDifferentRotations(m_originalItems, 0, best);
        } while (std::next_permutation(m_originalItems.begin(), m_originalItems.end(), [](const NestedItem& a, const NestedItem&b) {
            return a.getVariant()->getGlobalId(false) < b.getVariant()->getGlobalId(false);
        }));

    } else {
        int currIdx = m_originalItems[0].getVariant()->getGlobalId(false);
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

        std::sort(m_originalItems.begin(), m_originalItems.end(), [](const NestedItem&a, const NestedItem& b) {
            auto aBounds = a.getItemBoundingBoxAtPosition();
            auto bBounds = b.getItemBoundingBoxAtPosition();
            return (aBounds.bottom - aBounds.top) > (bBounds.bottom - bBounds.top);
        });

        best.result = std::move(runSingleBottomLeft(m_originalItems));
    }
    return best.result;
}

} // namespace pmg::nesting