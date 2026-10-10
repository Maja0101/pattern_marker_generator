#include "nesting/bottom_left.hpp"

namespace pmg::nesting {
bool BottomLeft::isCollision(const Clipper2Lib::Path64 &placed_piece)
{
    // check if piece collide with previously placed pieces
    return false;
}

bool BottomLeft::isWithinFabric(const Clipper2Lib::Path64 &placed_piece)
{
    // chcek if the whole piece is inside fabric
    return false;
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
    : m_fabric(std::move(fabric)), m_step_size(step_size) {

        m_fabric_clipper_path = m_fabric.toClipperPath();

        m_fabric_bounds = Clipper2Lib::GetBounds(m_fabric_clipper_path);
}

std::vector<PlacedInstance> BottomLeft::performNesting(std::vector<pattern::PieceVariant> variants)
{
    // perform the nesting for the pattern
    return std::vector<PlacedInstance>();
}

} // namespace pmg::nesting
