#include <unordered_map>

#include "geometry/options_calculations.hpp"

namespace pmg::geometry::calculations {

bool numberOfOptionsWithinLimit(const std::vector<nesting::NestedItem> &items, uint64_t limit)
{
    size_t n = items.size();
    if (n == 0) {
        return true;
    }

    size_t twoWayItemsCount = 0;
    std::unordered_map<int64_t, size_t> options;
    for (const auto& item : items) {
        if (item.getVariant()->getIsTwoWay()) {
            ++twoWayItemsCount;
        }
        ++options[item.getVariant()->getGlobalId(false)];
    }

    uint64_t total = 1;
    size_t currentN = 1;

    for (const auto& [id, freq] : options) {
        for (size_t i = 1; i <= freq; ++i) {
            total = (total * currentN) / i;
            ++currentN;

            if (total > limit) {
                return false;
            }
        }
    }

    if (twoWayItemsCount >= 64 || (1ULL << twoWayItemsCount) > limit) {
        return false;
    }

    uint64_t allowedTotal = limit >> twoWayItemsCount;
    if (total > allowedTotal) {
        return false;
    }

    total <<= twoWayItemsCount;
    
    return total <= limit;
}

double calculateNestingScore(const nesting::NestingStats &stats, const utils::ScoringWeights w)
{
    // Remaining fabric subscore
    double lengthRatioPcs = (stats.totalFabricLength > 0) ? (stats.remainingFabricLength / stats.totalFabricLength) * 100.0 : 0.0;
    double sFabric = w.alphaFabric * stats.remainingFabricPercentage + (1.0 - w.alphaFabric) * lengthRatioPcs;

    // Scraps subscore
    double sScrap = stats.usableScrapsPercentage;

    // Waste penalty
    double pWaste = stats.wastePercentage;

    // Fragmentation penalty
    double pFrag = 0.0;
    if (stats.usableScrapCount > 1) {
        pFrag = (1.0 - (1.0 / std::log(stats.usableScrapCount + 1))) * 100.0;
    }

    double totalScore = (w.wFabric * sFabric) +
                        (w.wScrap * sScrap) -
                        (w.wWaste * pWaste) -
                        (w.wFrag * pFrag);

    return std::max(0.0, std::min(100.0, totalScore));
}

} // namespace pmg::geometry::calculations 