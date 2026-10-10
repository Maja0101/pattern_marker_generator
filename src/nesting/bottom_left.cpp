#include "nesting/bottom_left.hpp"

namespace pmg::nesting {
bool BottomLeft::isCollision(const Clipper2Lib::Path64 &placed_piece)
{
    // check if piece collide with previously placed pieces
    return false;
}

bool BottomLeft::isWithinFabric(const Clipper2Lib::Path64 &placed_piece)
{
    Clipper2Lib::Rect64 piece_bounding_box = Clipper2Lib::GetBounds(placed_piece);

    // Fast check using bounding box
    const auto& fabricBounds = m_fabric.getBoundingBox();
    if (piece_bounding_box.left < fabricBounds.left || piece_bounding_box.right > fabricBounds.right ||
        piece_bounding_box.bottom < fabricBounds.bottom || piece_bounding_box.top > fabricBounds.top) {
            return false;
    }

    // Precise check 
    Clipper2Lib::Paths64 overflow = Clipper2Lib::Difference(
        Clipper2Lib::Paths64{ placed_piece },
        Clipper2Lib::Paths64{ m_fabric.getPath() },
        Clipper2Lib::FillRule::NonZero
    );

    return overflow.empty();
}

Clipper2Lib::Path64 BottomLeft::translate(const Clipper2Lib::Path64 &, int64_t dx, int64_t dy)
{
    // translate single piece by (dx, dy)
    return Clipper2Lib::Path64();
}

bool BottomLeft::findPosition(Clipper2Lib::Path64 piece)
{
    // find the position of the piece using Bottom Left Algorithm 
    return false;
}

BottomLeft::BottomLeft(pattern::Fabric fabric, int64_t step_size)
    : m_fabric(std::move(fabric)), m_step_size(step_size) {}

std::vector<PlacedInstance> BottomLeft::performNesting(std::vector<pattern::PieceVariant> variants)
{
    // perform the nesting for the pattern
    return std::vector<PlacedInstance>();
}

} // namespace pmg::nesting
