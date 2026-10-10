#pragma once

#include <vector>

#include <clipper2/clipper.h>

#include "pattern/shapes.hpp"
#include "nesting/placed_instance.hpp"

namespace pmg::nesting {

class BottomLeft {
private:
    pattern::Fabric m_fabric;

    int64_t m_step_size;

    std::vector<PlacedInstance> m_results;
    Clipper2Lib::Paths64 m_placed_pieces_polygons;

    bool isCollision(const Clipper2Lib::Path64& placed_piece);
    bool isWithinFabric(const Clipper2Lib::Path64& placed_piece);

    Clipper2Lib::Path64 translate(const Clipper2Lib::Path64&, int64_t dx, int64_t dy);

    bool findPosition(Clipper2Lib::Path64 piece);
public:
    BottomLeft(pattern::Fabric fabric, int64_t step_size = 1000);

    std::vector<PlacedInstance> performNesting(std::vector<pattern::PieceVariant> variants);

    std::vector<PlacedInstance> getResults() const noexcept { return m_results; }
};

} // namespace pmg::nesting