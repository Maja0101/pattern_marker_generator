#pragma once

#include <vector>

#include <clipper2/clipper.h>

#include "pattern/shapes.hpp"

namespace pmg::nesting {

struct BLStateView {
    const pattern::Fabric& fabric;
    const Clipper2Lib::Paths64& placed_pieces_polygons;
    const std::vector<Clipper2Lib::Rect64>& placed_pieces_boxes;
};

} // namespace pmg::pattern