#include <format>
#include <string>

#include "nesting/nested_item.hpp"

namespace pmg::nesting {

NestedItem::NestedItem(pattern::PieceVariant* variant) noexcept
    : m_position(geometry::Point64()), m_variantRef(variant) {}

Clipper2Lib::Path64 NestedItem::getItemPathAtPosition(int64_t x, int64_t y) const
{
    return m_variantRef->getPathForVariantAt(x, y, m_isFlipped);
}

Clipper2Lib::Path64 NestedItem::getItemPathAtPosition() const
{
    return getItemPathAtPosition(m_position.x, m_position.y);
}

Clipper2Lib::Rect64 NestedItem::getItemBoundingBoxAtPosition(int64_t x, int64_t y) const
{
    return m_variantRef->getBoundingBoxAt(x, y);
}

Clipper2Lib::Rect64 NestedItem::getItemBoundingBoxAtPosition() const
{
    return getItemBoundingBoxAtPosition(m_position.x, m_position.y);
}

bool NestedItem::lockInPosition(int64_t x, int64_t y)
{
    m_isLocked = true;
    m_position.x += x;
    m_position.y += y;

    return true;
}

bool NestedItem::resetPosition()
{
    m_isLocked = false;
    m_position.x = 0;
    m_position.y = 0;

    return true;
}

bool NestedItem::flip180()
{
    if ((m_variantRef->getIsTwoWay()) && (!m_isLocked)) {
        m_isFlipped = !m_isFlipped;
        return true;
    }
    return false;
}

geometry::Point64 NestedItem::getPositionForItemWithoutAllowances() const
{
    if (!isLocked()) return {};

    return geometry::Point64{
        m_position.x + m_variantRef->getAllowancesDistance(),
        m_position.y + m_variantRef->getAllowancesDistance()
    };
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