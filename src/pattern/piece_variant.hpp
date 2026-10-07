#pragma once

#include <vector>
#include "geometry/geometry_types.hpp"

namespace pmg::pattern {

struct PieceVariant {
    int rotationAngle{0};
    bool isMirrored{false};
    int requiredQuantity{0};
    // bounding box, etc.
    std::vector<geometry::Point64> transformedBoundary;
};

} // namespace pmg::pattern