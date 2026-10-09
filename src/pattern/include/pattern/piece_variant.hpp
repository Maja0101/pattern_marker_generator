#pragma once

#include <vector>
#include <string>

#include "clipper2/clipper.h"

#include "geometry/geometry_types.hpp"

namespace pmg::pattern {

struct PieceVariant {
    int rotationAngle{0};
    bool isMirrored{false};
    int requiredQuantity{0};
    std::vector<geometry::Point64> transformedBoundary;

    // toClipperPath
    std::string toString() const;
};

} // namespace pmg::pattern