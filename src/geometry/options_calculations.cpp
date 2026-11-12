#include <unordered_map>

#include <spdlog/spdlog.h>

#include "geometry/options_calculations.hpp"
#include "utils/config.hpp"

namespace pmg::geometry::calculations {

bool numberOfOptionsWithinLimit(const std::vector<nesting::NestedItem> &items, uint64_t limit)
{
    // Check number of permutations
    // Items with the same globalId are treated as equal
    // If the item is two-way the number of possibilities 
    // should be multiplied by two 

    size_t n = items.size();

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("items size {}", n));
    }

    if (n == 0) {
        return true;
    }

    // Calculate the number of each item and number of two-way items
    size_t twoWayItemsCount = 0;
    std::unordered_map<int64_t, size_t> options;
    for (const auto& item : items) {
        if (item.getVariant()->getIsTwoWay()) {
            ++twoWayItemsCount;
        }
        ++options[item.getVariant()->getGlobalId(false)];
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("two ways items count: {}", twoWayItemsCount));
        for (const auto& [id, freq] : options) {
            SPDLOG_INFO(std::format("id {} freq {}", id, freq));
        }
    }

    uint64_t total = 1;
    size_t currentN = 1;

    // Check if number of possibilites is above limit
    for (const auto& [id, freq] : options) {
        for (size_t i = 1; i <= freq; ++i) {
            total = (total * currentN) / i;
            ++currentN;

            if (total > limit) {

                if constexpr (config::DEBUG_MODE_DETAILED) {
                    SPDLOG_INFO(std::format("{}: total > limit", total));
                }

                return false;
            }
        }
    }

    // Check if number of two-way items is too large
    if (const size_t MAX_TWO_WAY_ITEMS = 64; twoWayItemsCount >= MAX_TWO_WAY_ITEMS || (1ULL << twoWayItemsCount) > limit) {
        
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("{}: two way item count too big", twoWayItemsCount));
        }
        
        return false;
    }

    // Check if number of possibilites is small enough to not exceed limit 
    // when two-way items taken into account
    uint64_t allowedTotal = limit >> twoWayItemsCount;

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("two way item count {} allowed total {} total {}", twoWayItemsCount, allowedTotal, total));
    }

    if (total > allowedTotal) {
        
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("{}: total > allowedTotal ({})", total, allowedTotal));
        }

        return false;
    }

    // Caclualte final umber of possibilites
    total <<= twoWayItemsCount;

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(total);
    }
    
    return total <= limit;
}

double calculateNestingScore(const nesting::NestingStats &stats, const utils::ScoringWeights &w)
{
    // Remaining fabric subscore
    double lengthRatioPcs = (stats.totalFabricLength > 0) ? (static_cast<double>(stats.remainingFabricLength) / static_cast<double>(stats.totalFabricLength)) * 100.0 : 0.0;
    double sFabric = (w.alphaFabric * stats.remainingFabricPercentage) + ((1.0 - w.alphaFabric) * lengthRatioPcs);

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