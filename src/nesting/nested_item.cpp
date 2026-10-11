#include <iostream>
#include <format>
#include <string>

#include "nesting/nested_item.hpp"

namespace pmg::nesting {

NestedItem::NestedItem(geometry::Point64 pos, const pattern::PieceVariant* variant) noexcept
    : m_position(pos), m_variantRef(variant) {}

std::string NestedItem::toString() const {
    if (!m_variantRef) return "";

    return std::format("Placed item at {} [{}]",
                       m_position.toString(),
                       m_variantRef->toString());
}

} // namespace pmg::nesting