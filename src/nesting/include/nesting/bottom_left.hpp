#pragma once

#include <vector>

#include <clipper2/clipper.h>

#include "pattern/shapes.hpp"
#include "nesting/nested_item.hpp"

namespace pmg::nesting {

class BottomLeft {
private:
    pattern::Fabric m_fabric;

    int64_t m_step_size;

    std::vector<NestedItem> m_results;
    Clipper2Lib::Paths64 m_placed_pieces_polygons;
    std::vector<Clipper2Lib::Rect64> m_placed_pieces_boxes;

    bool isCollision(const Clipper2Lib::Path64& piece_path, const Clipper2Lib::Rect64& piece_bounding_box);
    bool isWithinFabric(const Clipper2Lib::Path64& piece_path, const Clipper2Lib::Rect64& piece_bounding_box);

    Clipper2Lib::Path64 translate(const Clipper2Lib::Path64& path, int64_t dx, int64_t dy);
    bool aabbCollision(const Clipper2Lib::Rect64& a, const Clipper2Lib::Rect64& b);

    bool findPosition(NestedItem& item);
public:
    BottomLeft(pattern::Fabric fabric, int64_t step_size = 1000);

    std::vector<NestedItem> performNesting(std::vector<NestedItem> items);

    std::vector<NestedItem> getResults() const noexcept { return m_results; }
};

} // namespace pmg::nesting