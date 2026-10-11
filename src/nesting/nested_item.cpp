#include <iostream>
#include <format>
#include <string>

#include "nesting/nested_item.hpp"

namespace pmg::nesting {

NestedItem::NestedItem(geometry::Point64 pos, const pattern::PieceVariant* variant) noexcept
    : m_position(pos), m_variantRef(variant), m_isLocked(false) {}

const Clipper2Lib::Path64 NestedItem::getItemPathAt(int64_t x, int64_t y) const
{
    return m_variantRef->getPathAt(x, y);
}

const Clipper2Lib::Rect64 NestedItem::getItemBoundingBoxAt(int64_t x, int64_t y) const
{
    return m_variantRef->getBoundingBoxAt(x, y);
}

std::string NestedItem::toString() const
{
    if (!m_variantRef) return "";

    if (m_isLocked) {
        return std::format("Placed item at {} [{}]",
                            m_position.toString(),
                            m_variantRef->toString());
    }

    return std::format("Item to be placed [{}]",
                            m_position.toString(),
                            m_variantRef->toString());
}

} // namespace pmg::nesting