#pragma once

#include <vector>
#include <string>

#include "clipper2/clipper.h"

#include "geometry/geometry_types.hpp"

namespace pmg::pattern {

struct Shape {
    std::vector<geometry::Point64> boundary;

    Shape(std::vector<geometry::Point64> initalBoundary = {});
    virtual ~Shape() = default;

    Clipper2Lib::Path64 toClipperPath() const;
    virtual std::string toString() const;
};

struct Fabric : Shape {
    using Shape::Shape;
};

struct PieceVariant : Shape {
    int rotationAngle{0};
    bool isMirrored{false};
    int requiredQuantity{0};

    using Shape::Shape;

    std::string toString() const override;
};

} // namespace pmg::pattern