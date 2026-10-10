#include <iostream>
#include <format>
#include <string>

#include "nesting/placed_instance.hpp"

namespace pmg::nesting {

PlacedInstance::PlacedInstance(geometry::Point64 pos, const pattern::PieceVariant* variant) noexcept
    : m_position(pos), m_variantRef(variant) {}

std::string PlacedInstance::toString() const {
    if (!m_variantRef) return "";

    return std::format("Placed instance at {} [{}]",
                       m_position.toString(),
                       m_variantRef->toString());
}

} // namespace pmg::nesting