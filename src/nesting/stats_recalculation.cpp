#include <algorithm>
#include <map>
#include <queue>
#include <tuple>

#include <spdlog/spdlog.h>

#include "nesting/stats_recalculation.hpp"
#include "geometry_calculations/calculations.hpp"

namespace pmg::nesting {

pattern::NestingStats checkStatsFor(
    const pattern::Fabric &fabric, 
    std::vector<pattern::PatternPiece> &patternPieces, 
    const std::vector<pattern::ItemPlacement> newPlacements)
{
    std::vector<pattern::NestedItem> nestedItems(geometry::calculations::prepareItems(patternPieces));
    
    // Key consists of piece id, rotation angle and is mirrored
    using Key = std::tuple<int64_t, int, bool>;

    struct PlacementInfo {
        size_t index;
        bool isFlipped;
    };

    // Create a queue of placement info
    std::map<Key, std::queue<PlacementInfo>> orderMap;

    for (size_t i = 0; i < newPlacements.size(); ++i) {
        Key k = std::make_tuple(newPlacements[i].id, newPlacements[i].rotationAngle, newPlacements[i].isMirrored);
        
        orderMap[k].push({i, newPlacements[i].isFlipped180});
    }

    // Match each item to its placement and flip if needed
    std::vector<std::pair<size_t, pattern::NestedItem>> indexedItems;
    indexedItems.reserve(nestedItems.size());

    for (auto& item : nestedItems) {
        Key k = std::make_tuple(
            item.getVariant()->getPieceId(), 
            item.getVariant()->getRotationAngle(), 
            item.getVariant()->getIsMirrored()
        );
        
        auto it = orderMap.find(k);
        if (it != orderMap.end() && !it->second.empty()) {
            PlacementInfo info = it->second.front();
            it->second.pop();

            if (info.isFlipped) {
                item.flip180();
            }

            indexedItems.emplace_back(info.index, std::move(item));
        } else {
            indexedItems.emplace_back(newPlacements.size(), std::move(item));
        }
    }

    // Sort by target placement index
    std::sort(indexedItems.begin(), indexedItems.end(),
        [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

    // update sorted items to match indexedItems order
    for (size_t i = 0; i < indexedItems.size(); ++i) {
        nestedItems[i] = std::move(indexedItems[i].second);
    }

    // recalculate statistics
    return geometry::calculations::recalculateStats(fabric, nestedItems, newPlacements);
}

} // namespace pmg::nesting