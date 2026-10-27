#pragma once

#include "geometry/geometry_types.hpp"
#include "pattern/shapes.hpp"

namespace pmg::nesting {

class NestedItem {
private:
    geometry::Point64 m_position;
    pattern::PieceVariant* m_variantRef;
    bool m_isLocked;
    bool m_isFlipped;
public:
    NestedItem(pattern::PieceVariant* variant) noexcept;
    
    geometry::Point64 getPosition() const noexcept { return m_position; }
    const pattern::PieceVariant* getVariant() const noexcept { return m_variantRef; }
    bool isLocked() const noexcept { return m_isLocked; }

    Clipper2Lib::Path64 getItemPathAtPosition(int64_t x, int64_t y) const;
    Clipper2Lib::Path64 getItemPathAtPosition() const;
    Clipper2Lib::Rect64 getItemBoundingBoxAtPosition(int64_t x, int64_t y) const;
    Clipper2Lib::Rect64 getItemBoundingBoxAtPosition() const;

    bool lockInPosition(int64_t x, int64_t y);
    bool flip180();

    std::string toString() const;
};

} // namespace pmg::nesting