#pragma once

#include "pattern/geometry_types.hpp"
#include "pattern/shapes.hpp"

namespace pmg::pattern {

class NestedItem {
private:
    Point64 m_position;
    PieceVariant* m_variantRef;
    bool m_isLocked{false};
    bool m_isFlipped{false};
public:
    explicit NestedItem(PieceVariant* variant) noexcept;
    
    [[nodiscard]] Point64 getPosition() const noexcept { return m_position; }
    [[nodiscard]] const PieceVariant* getVariant() const noexcept { return m_variantRef; }
    [[nodiscard]] bool isLocked() const noexcept { return m_isLocked; }
    [[nodiscard]] bool isFlipped() const noexcept { return m_isFlipped; }
    [[nodiscard]] bool canBeFlipped() const noexcept { return m_variantRef->getIsTwoWay(); }

    [[nodiscard]] Clipper2Lib::Path64 getItemPathAtPosition(int64_t x, int64_t y) const;
    [[nodiscard]] Clipper2Lib::Path64 getItemPathAtPosition() const;
    [[nodiscard]] Clipper2Lib::Rect64 getItemBoundingBoxAtPosition(int64_t x, int64_t y) const;
    [[nodiscard]] Clipper2Lib::Rect64 getItemBoundingBoxAtPosition() const;

    bool lockInPosition(int64_t x, int64_t y);
    bool resetPosition();
    bool flip180();

    [[nodiscard]] Point64 getPositionForItemWithoutAllowances() const;

    [[nodiscard]] std::string toString() const;

    bool operator==(const NestedItem& other) const noexcept {
        return m_variantRef->getGlobalId(m_isFlipped) == other.getVariant()->getGlobalId(other.isFlipped());
    }
};

} // namespace pmg::nesting