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

} // namespace pmg::geometry::calculations 