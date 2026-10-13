#pragma once

#include <vector>

#include <clipper2/clipper.h>

#include "pattern/shapes.hpp"
#include "nesting/nested_item.hpp"
#include "nesting/bl_state.hpp"

namespace pmg::nesting {

class BottomLeft {
private:
    pattern::Fabric m_fabric;

    int64_t m_step_size;

    Clipper2Lib::Paths64 m_placed_pieces_polygons;
    std::vector<Clipper2Lib::Rect64> m_placed_pieces_boxes;

    BLStateView makeCurrStateView() const;

    bool findPosition(NestedItem& item);
public:
    BottomLeft(pattern::Fabric fabric, int64_t step_size = 1000);

    bool performNesting(std::vector<NestedItem>& items);
};

} // namespace pmg::nesting