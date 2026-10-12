#include <spdlog/spdlog.h>

#include "nesting/bottom_left.hpp"
#include "utils/config.hpp"

namespace pmg::nesting {

bool BottomLeft::aabbCollision(const Clipper2Lib::Rect64 &a, const Clipper2Lib::Rect64 &b)
{
    // if constexpr (config::DEBUG_MODE) {
    //     SPDLOG_CRITICAL(std::format("a l={}, r={}, b={}, t={}", a.left, a.right, a.bottom, a.top));
    //     SPDLOG_CRITICAL(std::format("b l={}, r={}, b={}, t={}", b.left, b.right, b.bottom, b.top));
    //     SPDLOG_CRITICAL(std::format("{} {} {} {}, {} {}, {}", 
    //         a.left <= b.right,
    //         a.right >= b.left,
    //         a.top <= b.bottom,
    //         a.bottom >= b.top,
    //         (a.left <= b.right && a.right >= b.left),
    //         (a.top <= b.bottom && a.bottom >= b.top),
    //         ((a.left <= b.right && a.right >= b.left) && (a.top <= b.bottom && a.bottom >= b.top))
    //     ));
    // } 
    return (a.left <= b.right && a.right >= b.left) && // (aMinX <= bMaxX && aMaxX >= bMinX) &&
            (a.top <= b.bottom && a.bottom >= b.top); // (aMinY <= bMaxY && aMaxY >= bMinY)
}

bool BottomLeft::isCollision(const NestedItem &item, int64_t x, int64_t y)
{
    if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO(std::format("Checking for collision at x={}, y={}", x, y));
    }
    
    // Bounding boxes of item at potential position
    Clipper2Lib::Rect64 itemBoundsAt = item.getItemBoundingBoxAtPosition(x, y);

    // Fast check using bounding boxes and store potential collision shapes
    Clipper2Lib::Paths64 potential_obstacles;

    for (size_t i = 0; i < m_placed_pieces_boxes.size(); ++i) {
        if (aabbCollision(itemBoundsAt, m_placed_pieces_boxes[i])) {
            potential_obstacles.push_back(m_placed_pieces_polygons[i]);
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

bool BottomLeft::isWithinFabric(const NestedItem &item, int64_t x, int64_t y)
{
    // Bounding boxes of fabric and item at potential position
    Clipper2Lib::Rect64 fabricBounds = m_fabric.getBoundingBox();
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
        Clipper2Lib::Paths64{ m_fabric.getPath() },
        Clipper2Lib::FillRule::NonZero
    );

    return overflow.empty();
}

bool BottomLeft::canPlaceAt(const NestedItem &item, int64_t x, int64_t y)
{
    if (!isWithinFabric(item, x, y)) {
        if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO("Item not wihin fabric");
        }
        return false;
    }

    if (isCollision(item, x, y)) {
        if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO("Item has collision");
        }
        return false;
    }

    return true;
}

bool BottomLeft::findPosition(NestedItem &item)
{
    // Calculate width and height of item's bounding box and determine the starting position
    Clipper2Lib::Rect64 fabricBounds = m_fabric.getBoundingBox();
    Clipper2Lib::Rect64 item_bounding_box = item.getVariant()->getBoundingBox();

    int64_t item_width = item_bounding_box.Width();
    int64_t item_height = item_bounding_box.Height();

    int64_t current_x = fabricBounds.left;
    int64_t current_y = fabricBounds.bottom - item_height; // In Clipper2 bottom = maxY, top = minY

    if constexpr (config::DEBUG_MODE) {
        SPDLOG_INFO(std::format("Item w={}, h={}", item_width, item_height));
        SPDLOG_INFO(std::format("Curr x={}, y={}", current_x, current_y));
    }

    // Verify starting position and move right if blocked
    while (!canPlaceAt(item, current_x, current_y)) {
        if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO("Starting point not valid, moving to the right");
        }
        current_x += m_step_size;
        if (current_x + item_width > fabricBounds.right) {
            if constexpr (config::DEBUG_MODE) {
                SPDLOG_INFO(std::format("Starting point {} is out of bounds, no possible position", current_x));
            }
            return false;
        }
    }

    // Down-Left loop
    bool wasMoved = true;
    while (wasMoved) {
        wasMoved = false;

        // Slide down 
        while (canPlaceAt(item, current_x, current_y - m_step_size)) {
            current_y -= m_step_size;
            wasMoved = true;
            if constexpr (config::DEBUG_MODE) {
                SPDLOG_INFO(std::format("Slide down, curr x={}, y={}", current_x, current_y));
            }
        }

        // Slide left
        while (canPlaceAt(item, current_x - m_step_size, current_y)) {
            current_x -= m_step_size;
            wasMoved = true;
            if constexpr (config::DEBUG_MODE) {
                SPDLOG_INFO(std::format("Slide left, curr x={}, y={}", current_x, current_y));
            }
        }
    }

    if constexpr (config::DEBUG_MODE) {
        SPDLOG_INFO(std::format("curr x={}, y={}", current_x, current_y));
    }

    // Lock item into place
    m_placed_pieces_polygons.push_back(item.getItemPathAtPosition(current_x, current_y));
    m_placed_pieces_boxes.push_back(item.getItemBoundingBoxAtPosition(current_x, current_y));
    item.lockInPosition(current_x, current_y);

    if constexpr (config::DEBUG_MODE) {
        SPDLOG_INFO("Item locked");
        SPDLOG_INFO(item.toString());
    }

    return true;
}

BottomLeft::BottomLeft(pattern::Fabric fabric, int64_t step_size)
    : m_fabric(std::move(fabric)), m_step_size(step_size) {}

bool BottomLeft::performNesting(std::vector<NestedItem> &items)
{
    if constexpr (config::DEBUG_MODE) {
        SPDLOG_INFO("Starting nesting");
    }

    int i = 1;
    for (auto& item : items) {
        // if (i > 2) {
        //     throw std::invalid_argument("the next item");
        // }
        if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO(std::format("Nesting for {}", item.toString()));
        }

        if (!findPosition(item)) {
            if constexpr (config::DEBUG_MODE) {
                SPDLOG_INFO("Could not find position");
            }
            return false;
        }
        i++;
    }

    return true;
}

} // namespace pmg::nesting
