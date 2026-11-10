#pragma once

#include "geometry/geometry_types.hpp"
#include "pattern/shapes.hpp"

namespace pmg::nesting {

class NestedItem {
private:
    geometry::Point64 m_position;
    pattern::PieceVariant* m_variantRef;
    bool m_isLocked{false};
    bool m_isFlipped{false};
public:
    NestedItem(pattern::PieceVariant* variant) noexcept;
    
    [[nodiscard]] geometry::Point64 getPosition() const noexcept { return m_position; }
    [[nodiscard]] const pattern::PieceVariant* getVariant() const noexcept { return m_variantRef; }
    [[nodiscard]] bool isLocked() const noexcept { return m_isLocked; }
    [[nodiscard]] bool isFlipped() const noexcept { return m_isFlipped; }

    [[nodiscard]] Clipper2Lib::Path64 getItemPathAtPosition(int64_t x, int64_t y) const;
    [[nodiscard]] Clipper2Lib::Path64 getItemPathAtPosition() const;
    [[nodiscard]] Clipper2Lib::Rect64 getItemBoundingBoxAtPosition(int64_t x, int64_t y) const;
    [[nodiscard]] Clipper2Lib::Rect64 getItemBoundingBoxAtPosition() const;

    bool lockInPosition(int64_t x, int64_t y);
    bool resetPosition();
    bool flip180();

    [[nodiscard]] geometry::Point64 getPositionForItemWithoutAllowances() const;

    [[nodiscard]] std::string toString() const;
};

} // namespace pmg::nesting