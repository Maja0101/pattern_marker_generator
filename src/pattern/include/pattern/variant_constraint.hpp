#pragma once

namespace pmg::pattern {

struct VariantConstraint {
    int rotationAngle{0};
    bool isMirrored{false};
    int requiredQuantity{1};
};

} // namespace pmg::pattern