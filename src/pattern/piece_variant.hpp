#pragma once

#include <vector>
#include <format>
#include <string>
#include <numeric>
#include <ranges>

// #include <clipper2/clipper.h>

#include "geometry/geometry_types.hpp"

namespace pmg::pattern {

struct PieceVariant {
    int rotationAngle{0};
    bool isMirrored{false};
    int requiredQuantity{0};
    std::vector<geometry::Point64> transformedBoundary;

    std::string toString() const {
        std::string boundaryStr = std::accumulate(
            std::next(transformedBoundary.begin()), transformedBoundary.end(),
            transformedBoundary.front().toString(),
            [](std::string a, geometry::Point64 b) {
                return a + ", " + b.toString();
            }
        );

        return std::format("rotationAngle: {}, isMirrored: {}, requiredQuantity: {}, boundary: ({})",
                            rotationAngle,
                            isMirrored,
                            requiredQuantity,
                            boundaryStr);
    }
};

} // namespace pmg::pattern