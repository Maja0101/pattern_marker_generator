#include "nesting/bottom_left.hpp"

namespace pmg::nesting {
bool BottomLeft::isCollision(const Clipper2Lib::Path64 &piece_path, const Clipper2Lib::Rect64 &piece_bounding_box)
{
    // TODO might need change; check first if the piece does not collide at the position and then move to the position

    // Fast check using bounding boxes and store potential collision shapes
    Clipper2Lib::Paths64 potential_obstacles;
    for (size_t i = 0; i < m_placed_pieces_boxes.size(); ++i) {
        if (aabbCollision(piece_bounding_box, m_placed_pieces_boxes[i])) {
            potential_obstacles.push_back(m_placed_pieces_polygons[i]);
        }
    }

    // No collision if there are no overlapping bounding boxes
    if (potential_obstacles.empty()) {
        return false;
    }

    // Detailed intersection check
    Clipper2Lib::Paths64 piece_paths = Clipper2Lib::Paths64{ piece_path };

    Clipper2Lib::Paths64 intersection = Clipper2Lib::Intersect(
        piece_paths, 
        potential_obstacles, 
        Clipper2Lib::FillRule::NonZero
    );

    return !intersection.empty();
}

bool BottomLeft::isWithinFabric(const Clipper2Lib::Path64 &piece_path, const Clipper2Lib::Rect64 &piece_bounding_box)
{
    // Fast check using bounding box
    const auto& fabricBounds = m_fabric.getBoundingBox();
    if (piece_bounding_box.left < fabricBounds.left || piece_bounding_box.right > fabricBounds.right ||
        piece_bounding_box.bottom < fabricBounds.bottom || piece_bounding_box.top > fabricBounds.top) {
            return false;
    }

    // Precise check using difference between two paths
    Clipper2Lib::Paths64 overflow = Clipper2Lib::Difference(
        Clipper2Lib::Paths64{ piece_path },
        Clipper2Lib::Paths64{ m_fabric.getPath() },
        Clipper2Lib::FillRule::NonZero
    );

    return overflow.empty();
}

Clipper2Lib::Path64 BottomLeft::translate(const Clipper2Lib::Path64 &path, int64_t dx, int64_t dy)
{
    Clipper2Lib::Path64 result;
    result.reserve(path.size());
    for (const auto& point : path) {
        result.push_back(Clipper2Lib::Point64(point.x + dx, point.y + dy));
    }
    return result;
}

bool BottomLeft::aabbCollision(const Clipper2Lib::Rect64 &a, const Clipper2Lib::Rect64 &b)
{
    return (a.left <= b.right && a.right >= b.left) &&
            (a.bottom <= b.top && a.top >= b.bottom);
}

bool BottomLeft::findPosition(NestedItem &item)
{

    // find the position of the piece using Bottom Left Algorithm 
    return false;
}

BottomLeft::BottomLeft(pattern::Fabric fabric, int64_t step_size)
    : m_fabric(std::move(fabric)), m_step_size(step_size) {}

std::vector<NestedItem> BottomLeft::performNesting(std::vector<NestedItem> items)
{
    // perform the nesting for the pattern
    return std::vector<NestedItem>();
}

} // namespace pmg::nesting
