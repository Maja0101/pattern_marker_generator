#pragma once

#include <vector>
#include <string>

#include "clipper2/clipper.h"

#include "geometry/geometry_types.hpp"

namespace pmg::pattern {

class Shape {
protected:
    std::vector<geometry::Point64> m_boundary;
    Clipper2Lib::Path64 m_shape_path;
    Clipper2Lib::Rect64 m_shape_bounding_box;
public:
    Shape(std::vector<geometry::Point64> initalBoundary = {});
    Shape(int64_t width, int64_t length);
    Shape(Clipper2Lib::Path64 initalPath);
    virtual ~Shape() = default;
    
    const std::vector<geometry::Point64>& getBoundary() const { return m_boundary; }
    const Clipper2Lib::Path64& getPath() const { return m_shape_path; }
    const Clipper2Lib::Rect64& getBoundingBox() const { return m_shape_bounding_box; }
    
    const Clipper2Lib::Path64 getPathAt(int64_t x, int64_t y) const;
    const Clipper2Lib::Rect64 getBoundingBoxAt(int64_t x, int64_t y) const;

    virtual std::string toString() const;
};

class Fabric : public Shape {
public:
    using Shape::Shape;
};

class PieceVariant : public Shape {
private:
    // TODO two-way pattern pieces
    int64_t m_globalId;
    int m_id;
    int m_rotationAngle;
    bool m_isMirrored;
    int m_requiredQuantity;
    bool isTwoWay;
    int64_t m_seamAllowance;
    int64_t m_cuttingAllowance;
    bool m_isConvex;
    std::vector<Clipper2Lib::Path64> m_decomposedShape;

    void initializeVariantGeometry();
public:
    PieceVariant(int64_t globalId, int id, int rotationAngle = 0, bool isMirrored = false, int requiredQuantity = 1, bool twoWayAllowed = false, int64_t seamAllowance = 0, int64_t cuttingAllowance = 0, std::vector<geometry::Point64> initalBoundary = {});
    PieceVariant(Clipper2Lib::Path64 initalPath = {});

    const std::vector<Clipper2Lib::Path64>& getDecomposedShape() const { return m_decomposedShape; }

    const int64_t getGlobalId() const { return m_globalId; }
    const int getId() const { return m_id; }
    const int getRotationAngle() const { return m_rotationAngle; }
    const bool getIsMirrored() const { return m_isMirrored; }
    const int getRequiredQuantity() const { return m_requiredQuantity; }
    const int64_t getSeamAllowance() const { return m_seamAllowance; }
    const int64_t getCuttingAllowance() const { return m_cuttingAllowance; }
    const int getIsConvex() const { return m_isConvex; }

    bool flip180();

    std::string toString() const override;
};

} // namespace pmg::pattern