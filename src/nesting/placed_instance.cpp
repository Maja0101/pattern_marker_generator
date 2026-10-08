#include "placed_instance.hpp"
#include <iostream>
#include <format>
#include <string>

namespace pmg::nesting {

PlacedInstance::PlacedInstance(geometry::Point64 pos, const pattern::PieceVariant* variant) noexcept
    : position(pos), variantRef(variant) {}

std::string PlacedInstance::toString() const {
    if (!variantRef) return "";

    return std::format("Placed instance at {} [{}]",
                       position.toString(),
                       variantRef->toString());
}

} // namespace pmg::nesting