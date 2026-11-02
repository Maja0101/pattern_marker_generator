#include <unordered_map>

#include <spdlog/spdlog.h>

#include "geometry/options_calculations.hpp"
#include "utils/config.hpp"

namespace pmg::geometry::calculations {

bool numberOfOptionsWithinLimit(const std::vector<nesting::NestedItem> &items, uint64_t limit)
{
    size_t n = items.size();

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("items size {}", n));
    }

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

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("twoWayItemsCount {}", twoWayItemsCount));
        for (const auto& [id, freq] : options) {
            SPDLOG_INFO(std::format("id {} freq {}", id, freq));
        }
    }

    uint64_t total = 1;
    size_t currentN = 1;

    for (const auto& [id, freq] : options) {
        for (size_t i = 1; i <= freq; ++i) {
            total = (total * currentN) / i;
            ++currentN;

            if (total > limit) {
                if constexpr (config::DEBUG_MODE) {
                    SPDLOG_INFO(std::format("total > limit ({})", total));
                }
                return false;
            }
        }
    }

    if (twoWayItemsCount >= 64 || (1ULL << twoWayItemsCount) > limit) {
        if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO(std::format("twoWayItemsCount too big ({})", twoWayItemsCount));
        }
        return false;
    }

    uint64_t allowedTotal = limit >> twoWayItemsCount;
    if (total > allowedTotal) {
        if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO(std::format("total > allowedTotal ({} > {})", total, allowedTotal));
        }
        return false;
    }

    total <<= twoWayItemsCount;

    if constexpr (config::DEBUG_MODE) {
        SPDLOG_INFO(total);
    }
    
    return total <= limit;
}

double calculateNestingScore(const nesting::NestingStats &stats, const utils::ScoringWeights w)
{
    // Remaining fabric subscore
    double lengthRatioPcs = (stats.totalFabricLength > 0) ? (stats.remainingFabricLength / stats.totalFabricLength) * 100.0 : 0.0;
    double sFabric = w.alphaFabric * stats.remainingFabricPercentage + (1.0 - w.alphaFabric) * lengthRatioPcs;

    // Scraps subscore
    double sScrap = stats.effectiveScrapsPercentage;

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