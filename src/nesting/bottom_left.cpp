#include "nesting/bottom_left.hpp"

namespace pmg::nesting {

bool BottomLeft::aabbCollision(const Clipper2Lib::Rect64 &a, const Clipper2Lib::Rect64 &b)
{
    return (a.left <= b.right && a.right >= b.left) &&
            (a.bottom <= b.top && a.top >= b.bottom);
}

bool BottomLeft::isCollision(const NestedItem &item, int64_t x, int64_t y)
{
    // Bounding boxes of item at potential position
    Clipper2Lib::Rect64 itemBoundsAt = item.getItemBoundingBoxAtPosition(x, y);

    // Fast check using bounding boxes and store potential collision shapes
    Clipper2Lib::Paths64 potential_obstacles;

    for (size_t i = 0; i < m_placed_pieces_boxes.size(); ++i) {
        if (aabbCollision(itemBoundsAt, m_placed_pieces_boxes[i])) {
            potential_obstacles.push_back(m_placed_pieces_polygons[i]);
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
        itemBoundsAt.bottom < fabricBounds.bottom || itemBoundsAt.top > fabricBounds.top) {
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
        return false;
    }

    if (isCollision(item, x, y)) {
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
    int64_t current_y = fabricBounds.top - item_height;

    // Verify starting position and move right if blocked
    while (!canPlaceAt(item, current_x, current_y)) {
        current_x += m_step_size;
        if (current_x + item_width > fabricBounds.right) {
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
        }

        // Slide left
        while (canPlaceAt(item, current_x - m_step_size, current_y)) {
            current_x -= m_step_size;
            wasMoved = true;
        }
    }

    // Lock item into place
    m_placed_pieces_polygons.push_back(item.getItemPathAtPosition(current_x, current_y));
    m_placed_pieces_boxes.push_back(item.getItemBoundingBoxAtPosition(current_x, current_y));
    item.lockInPosition(current_x, current_y);

    return true;
}

BottomLeft::BottomLeft(pattern::Fabric fabric, int64_t step_size)
    : m_fabric(std::move(fabric)), m_step_size(step_size) {}

bool BottomLeft::performNesting(std::vector<NestedItem> &items)
{
    for (auto& item : items) {
        if (!findPosition(item)) {
            return false;
        }
    }

    return true;
}

} // namespace pmg::nesting
