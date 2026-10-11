#pragma once

#include "geometry/geometry_types.hpp"
#include "pattern/shapes.hpp"

namespace pmg::nesting {

class NestedItem {
private:
    geometry::Point64 m_position;
    const pattern::PieceVariant* m_variantRef;
    bool m_isLocked;
public:
    NestedItem(geometry::Point64 pos, const pattern::PieceVariant* variant) noexcept;
    
    geometry::Point64 getPosition() const noexcept { return m_position; }
    const pattern::PieceVariant* getVariant() const noexcept { return m_variantRef; }

    const Clipper2Lib::Path64 getItemPathAt(int64_t x, int64_t y) const;
    const Clipper2Lib::Rect64 getItemBoundingBoxAt(int64_t x, int64_t y) const;

    std::string toString() const;
};

} // namespace pmg::nesting