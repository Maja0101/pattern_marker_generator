#pragma once

#include <vector>
#include <string>

#include <clipper2/clipper.h>

#include "geometry/geometry_types.hpp"

namespace pmg::pattern {

class Shape {
protected:
    Clipper2Lib::Path64 m_shapePath;
    std::vector<geometry::Point64> m_boundary;
    Clipper2Lib::Rect64 m_shapeBoundingBox;
public:
    explicit Shape(const std::vector<geometry::Point64>& initalBoundary = {}, bool toBeScaled = false);
    Shape(int64_t width, int64_t length, bool toBeScaled = false);
    explicit Shape(const Clipper2Lib::Path64& initalPath);
    virtual ~Shape() = default;
    
    [[nodiscard]] virtual const std::vector<geometry::Point64>& getBoundary() const { return m_boundary; }
    [[nodiscard]] virtual const Clipper2Lib::Path64& getPath() const { return m_shapePath; }
    [[nodiscard]] const Clipper2Lib::Rect64& getBoundingBox() const { return m_shapeBoundingBox; }
    
    [[nodiscard]] virtual Clipper2Lib::Path64 getPathAt(int64_t x, int64_t y) const;
    [[nodiscard]] Clipper2Lib::Rect64 getBoundingBoxAt(int64_t x, int64_t y) const;

    [[nodiscard]] virtual std::string toString() const;
};

class Fabric : public Shape {
public:
    using Shape::Shape;
};

class PieceVariant : public Shape {
private:
    int64_t m_pieceId;
    int m_variantId;
    int m_rotationAngle;
    bool m_isMirrored;
    int m_requiredQuantity;
    bool m_isTwoWay;
    int64_t m_seamAllowance;
    int64_t m_cuttingAllowance;
    geometry::Point64 m_referencePoint;
    bool m_isConvex;
    std::vector<Clipper2Lib::Path64> m_decomposedShape;

    std::vector<geometry::Point64> m_flippedBoundary;
    Clipper2Lib::Path64 m_flippedShapePath;
    std::vector<Clipper2Lib::Path64> m_decomposedFlippedShape;

    void initializeVariantGeometry();
public:
    PieceVariant(int variantId, int64_t pieceId, int rotationAngle = 0, bool isMirrored = false, 
        int requiredQuantity = 1, bool twoWayAllowed = false, 
        int64_t seamAllowance = 0, int64_t cuttingAllowance = 0, 
        const std::vector<geometry::Point64>& initalBoundary = {},
        geometry::Point64 referencePoint = geometry::Point64());
    explicit PieceVariant(const Clipper2Lib::Path64& initalPath);
    
    [[nodiscard]] int64_t getGlobalId(bool isFlipped) const;
    
    [[nodiscard]] int64_t getPieceId() const { return m_pieceId; }
    [[nodiscard]] int getVariantId() const { return m_variantId; }
    [[nodiscard]] int getRotationAngle() const { return m_rotationAngle; }
    [[nodiscard]] bool getIsMirrored() const { return m_isMirrored; }
    [[nodiscard]] int getRequiredQuantity() const { return m_requiredQuantity; }
    [[nodiscard]] int getIsTwoWay() const { return m_isTwoWay; }
    [[nodiscard]] int64_t getSeamAllowance() const { return m_seamAllowance; }
    [[nodiscard]] int64_t getCuttingAllowance() const { return m_cuttingAllowance; }
    [[nodiscard]] int getIsConvex() const { return m_isConvex; }
    [[nodiscard]] int64_t getAllowancesDistance() const { return m_seamAllowance + m_cuttingAllowance; }
    [[nodiscard]] geometry::Point64 getReferencePoint() const { return m_referencePoint; }
    
    [[nodiscard]] const std::vector<geometry::Point64>& getBoundary(bool isFlipped) const;
    [[nodiscard]] const Clipper2Lib::Path64& getPath(bool isFlipped) const;
    [[nodiscard]] Clipper2Lib::Path64 getPathAt(int64_t x, int64_t y, bool isFlipped) const;
    [[nodiscard]] const std::vector<Clipper2Lib::Path64>& getDecomposedShape(bool isFlipped) const;

    [[nodiscard]] std::string toString() const override;
};

} // namespace pmg::pattern