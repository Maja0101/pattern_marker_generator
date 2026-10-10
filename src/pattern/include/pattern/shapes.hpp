#pragma once

#include <vector>
#include <string>

#include "clipper2/clipper.h"

#include "geometry/geometry_types.hpp"

namespace pmg::pattern {

class Shape {
private:
    Clipper2Lib::Path64 toClipperPath() const;
protected:
    std::vector<geometry::Point64> m_boundary;
    Clipper2Lib::Path64 m_shape_path;
    Clipper2Lib::Rect64 m_shape_bounding_box;
public:
    Shape(std::vector<geometry::Point64> initalBoundary = {});
    virtual ~Shape() = default;
    
    const std::vector<geometry::Point64> getBoundary() const { return m_boundary; }
    const Clipper2Lib::Path64 getPath() const { return m_shape_path; }
    const Clipper2Lib::Rect64 getBoundingBox() const { return m_shape_bounding_box; }
    
    virtual std::string toString() const;
};

class Fabric : public Shape {
public:
    using Shape::Shape;
};

class PieceVariant : public Shape {
private:
    int m_rotationAngle;
    bool m_isMirrored;
    int m_requiredQuantity;
public:
    PieceVariant(int rotationAngle = 0, bool isMirrored = false, int requiredQuantity = 1, std::vector<geometry::Point64> initalBoundary = {});

    const int getRotationAngle() const { return m_rotationAngle; }
    const bool getIsMirrored() const { return m_isMirrored; }
    const int getRequiredQuantity() const { return m_requiredQuantity; }

    std::string toString() const override;
};

} // namespace pmg::pattern