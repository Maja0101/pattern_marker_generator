#include "placed_instance.hpp"
#include <iostream>

namespace pmg::nesting {

PlacedInstance::PlacedInstance(geometry::Point64 pos, const pattern::PieceVariant* variant) noexcept
    : position(pos), variantRef(variant) {}

void PlacedInstance::render() const {
    if (!variantRef) return;

    std::cout << "Placed variant at (" << position.x << ", " << position.y << ") "
              << "[Angle: " << variantRef->rotationAngle << " deg, Mirrored: " 
              << std::boolalpha << variantRef->isMirrored << "]\n";
}

} // namespace pmg::nesting