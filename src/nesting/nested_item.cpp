#include <iostream>
#include <format>
#include <string>

#include "nesting/nested_item.hpp"

namespace pmg::nesting {

NestedItem::NestedItem(const pattern::PieceVariant* variant) noexcept
    : m_position(geometry::Point64()), m_variantRef(variant), m_isLocked(false) {}

const Clipper2Lib::Path64 NestedItem::getItemPathAtPosition(int64_t x, int64_t y) const
{
    return m_variantRef->getPathAt(x, y);
}

const Clipper2Lib::Path64 NestedItem::getItemPathAtPosition() const
{
    return getItemPathAtPosition(m_position.x, m_position.y);;
}

const Clipper2Lib::Rect64 NestedItem::getItemBoundingBoxAtPosition(int64_t x, int64_t y) const
{
    return m_variantRef->getBoundingBoxAt(x, y);
}

const Clipper2Lib::Rect64 NestedItem::getItemBoundingBoxAtPosition() const
{
    return getItemBoundingBoxAtPosition(m_position.x, m_position.y);
}

void NestedItem::lockInPosition(int64_t x, int64_t y)
{
    m_isLocked = true;
    m_position.x += x;
    m_position.y += y;
}

std::string NestedItem::toString() const
{
    if (!m_variantRef) return "";

    if (m_isLocked) {
        return std::format("Placed item at {} [{}]",
                            m_position.toString(),
                            m_variantRef->toString());
    }

    return std::format("Item to be placed {} [{}]",
                            m_position.toString(),
                            m_variantRef->toString());
}

} // namespace pmg::nesting