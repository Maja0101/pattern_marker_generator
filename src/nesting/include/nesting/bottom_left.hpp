#pragma once

#include <vector>

#include <clipper2/clipper.h>

#include "pattern/shapes.hpp"
#include "nesting/nested_item.hpp"
#include "nesting/bl_state.hpp"
#include "geometry/nfp_key.hpp"

namespace pmg::nesting {

class BottomLeft {
private:
    pattern::Fabric m_fabric;

    std::unordered_map<int64_t, Clipper2Lib::Paths64>& m_ifpCache;
    std::unordered_map<geometry::NFPKey, Clipper2Lib::Paths64>& m_nfpCache;

    int64_t m_step_size;

    Clipper2Lib::Paths64 m_placed_pieces_polygons;
    std::vector<Clipper2Lib::Rect64> m_placed_pieces_boxes;

    BLStateView makeCurrStateView() const;

    bool findPosition(NestedItem& item, std::vector<NestedItem>& items);
public:
    BottomLeft(pattern::Fabric fabric, std::unordered_map<int64_t, Clipper2Lib::Paths64>& ifpCache, std::unordered_map<geometry::NFPKey, Clipper2Lib::Paths64>& nfpCache);

    bool performNesting(std::vector<NestedItem>& items);
};

} // namespace pmg::nesting