#include <spdlog/spdlog.h>

#include "nesting/bottom_left.hpp"
#include "utils/config.hpp"
#include "nesting/bl_helpers.hpp"

namespace pmg::nesting::calculations {

bool aabbCollision(const Clipper2Lib::Rect64 &a, const Clipper2Lib::Rect64 &b)
{
    return (a.left <= b.right && a.right >= b.left) && // (aMinX <= bMaxX && aMaxX >= bMinX) &&
            (a.top <= b.bottom && a.bottom >= b.top); // (aMinY <= bMaxY && aMaxY >= bMinY)
}

bool isCollision(const BLStateView &view, const NestedItem &item, int64_t x, int64_t y)
{
    if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO(std::format("Checking for collision at x={}, y={}", x, y));
    }
    
    // Bounding boxes of item at potential position
    Clipper2Lib::Rect64 itemBoundsAt = item.getItemBoundingBoxAtPosition(x, y);

    // Fast check using bounding boxes and store potential collision shapes
    Clipper2Lib::Paths64 potential_obstacles;

    for (size_t i = 0; i < view.placed_pieces_boxes.size(); ++i) {
        if (aabbCollision(itemBoundsAt, view.placed_pieces_boxes[i])) {
            potential_obstacles.push_back(view.placed_pieces_polygons[i]);
            if constexpr (config::DEBUG_MODE) {
                SPDLOG_WARN(std::format("Potential obstacle found", x, y));
            }   
        }
    }

    // No collision if there are no overlapping bounding boxes
    if (potential_obstacles.empty()) {
        return false;
    }

    // Item path at potential position
    Clipper2Lib::Path64 itemPathAt = item.getItemPathAtPosition(x, y);

    // Detailed intersection check
    Clipper2Lib::Paths64 intersection = Clipper2Lib::Intersect(
        Clipper2Lib::Paths64{ itemPathAt }, 
        potential_obstacles, 
        Clipper2Lib::FillRule::NonZero
    );

    return !intersection.empty();
}

bool isWithinFabric(const BLStateView &view, const NestedItem &item, int64_t x, int64_t y)
{
    // Bounding boxes of fabric and item at potential position
    Clipper2Lib::Rect64 fabricBounds = view.fabric.getBoundingBox();
    Clipper2Lib::Rect64 itemBoundsAt = item.getItemBoundingBoxAtPosition(x, y);

    // Fast check using bounding box
    if (itemBoundsAt.left < fabricBounds.left || itemBoundsAt.right > fabricBounds.right ||
        itemBoundsAt.bottom > fabricBounds.bottom || itemBoundsAt.top < fabricBounds.top) { // bottom is maxY, top is minY
            return false;
    }

    // Item path at potential position
    Clipper2Lib::Path64 itemPathAt = item.getItemPathAtPosition(x, y);

    // Precise check using difference between two paths
    Clipper2Lib::Paths64 overflow = Clipper2Lib::Difference(
        Clipper2Lib::Paths64{ itemPathAt },
        Clipper2Lib::Paths64{ view.fabric.getPath() },
        Clipper2Lib::FillRule::NonZero
    );

    return overflow.empty();
}

bool canPlaceAt(const BLStateView &view, const NestedItem &item, int64_t x, int64_t y)
{
    if (!isWithinFabric(view, item, x, y)) {
        if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO("Item not wihin fabric");
        }
        return false;
    }

    if (isCollision(view, item, x, y)) {
        if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO("Item has collision");
        }
        return false;
    }

    return true;
}

} // namespace pmg::nesting::calculations