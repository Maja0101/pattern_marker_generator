#include <format>
#include <numeric>
#include <ranges>

#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "geometry/decomposition_calculations.hpp"
#include "geometry/calculations.hpp"
#include "geometry/transformations.hpp"
#include "pattern/shapes.hpp"

namespace pmg::pattern {

Shape::Shape(std::vector<geometry::Point64> initalBoundary)
    : m_shapePath(Clipper2Lib::SimplifyPath(geometry::calculations::toClipperPath(initalBoundary), 0.0)),
      m_boundary((m_shapePath.size() == initalBoundary.size()) ? std::move(initalBoundary) : geometry::calculations::fromClipperPath(m_shapePath)),
      m_shapeBoundingBox(Clipper2Lib::GetBounds(m_shapePath)) {}

Shape::Shape(int64_t width, int64_t length)
    : m_shapePath([width, length]() {
        constexpr int64_t maxValue = 100000001;
        int64_t w = (width < 0) ? 0 : (width % maxValue);
        int64_t l = (length < 0) ? 0 : (length % maxValue);
        Clipper2Lib::Path64 temp;
        temp.reserve(4);
        temp.push_back({0L, 0L});
        temp.push_back({l, 0L});
        temp.push_back({l, w});
        temp.push_back({0L, w});
        return temp;
      }()), 
      m_boundary(geometry::calculations::fromClipperPath(m_shapePath)),
      m_shapeBoundingBox(Clipper2Lib::GetBounds(m_shapePath)) {}

Shape::Shape(Clipper2Lib::Path64 initalPath)
    : m_shapePath(Clipper2Lib::SimplifyPath(initalPath, 0.0)),
      m_boundary(geometry::calculations::fromClipperPath(m_shapePath)),
      m_shapeBoundingBox(Clipper2Lib::GetBounds(m_shapePath)) {}

const Clipper2Lib::Path64 Shape::getPathAt(int64_t x, int64_t y) const
{
    Clipper2Lib::Path64 tranlated_path;

    tranlated_path.reserve(m_shapePath.size());

    for (const auto& point : m_shapePath) {
        tranlated_path.push_back(Clipper2Lib::Point64(point.x + x, point.y + y));
    }
    return tranlated_path;
}

const Clipper2Lib::Rect64 Shape::getBoundingBoxAt(int64_t x, int64_t y) const
{
    Clipper2Lib::Rect64 tranlated_box = m_shapeBoundingBox;

    tranlated_box.left   += x;
    tranlated_box.right  += x;
    tranlated_box.top    += y;
    tranlated_box.bottom += y;

    return tranlated_box;
}

std::string Shape::toString() const
{
    std::string boundaryStr = std::accumulate(
        std::next(m_boundary.begin()), m_boundary.end(),
        m_boundary.front().toString(),
        [](std::string a, geometry::Point64 b) {
            return a + ", " + b.toString();
        }
    );

    return std::format("boundary: ({})", boundaryStr);
}

void PieceVariant::initializeVariantGeometry()
{
    m_isConvex = geometry::calculations::checkConvexity(m_boundary);

    if (m_isTwoWay) {
        std::vector<pmg::geometry::Point64> rotatedBoundary = geometry::calculations::rotateBy180(m_boundary);
        if constexpr (config::DEBUG_MODE_DETAILED) {
            for (const auto& point : m_boundary) {
                SPDLOG_INFO(std::format("boundary point {}", point.toString()));
            }
            for (const auto& point : rotatedBoundary) {
                SPDLOG_INFO(std::format("rotated boundary point {}", point.toString()));
            }
        }
        if (geometry::calculations::isCyclicShift(m_boundary, rotatedBoundary)) {
            m_isTwoWay = false;
        } else {
            m_flippedBoundary = geometry::calculations::rotateBy180(m_boundary);
            m_flippedShapePath = geometry::calculations::toClipperPath(m_flippedBoundary);
        }
    }

    if (m_isConvex) {
        m_decomposedShape = { m_shapePath };
        if (m_isTwoWay) {
            m_decomposedFlippedShape = { m_flippedShapePath };
        }
    } else {
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("decomposing {}", this->toString()));
        }
        std::vector<std::vector<pmg::geometry::Point64>> subShapes = geometry::calculations::hertelMehlhornAlgorithm(m_boundary);

        for (const auto& subShape : subShapes) {
            m_decomposedShape.push_back(geometry::calculations::toClipperPath(subShape));
        }

        if (m_isTwoWay) {
            m_decomposedFlippedShape = geometry::calculations::rotatePathsBy180(m_decomposedShape, m_shapeBoundingBox.right, m_shapeBoundingBox.bottom);
        }
    }
}

PieceVariant::PieceVariant(int variantId, int64_t pieceId, int rotationAngle, bool isMirrored, int requiredQuantity, bool twoWayAllowed, int64_t seamAllowance, int64_t cuttingAllowance, std::vector<geometry::Point64> initalBoundary)
    : Shape(initalBoundary), m_variantId(variantId), m_pieceId(pieceId), 
      m_rotationAngle(rotationAngle), m_isMirrored(isMirrored), 
      m_requiredQuantity(requiredQuantity), m_isTwoWay(twoWayAllowed),
      m_seamAllowance(seamAllowance), m_cuttingAllowance(cuttingAllowance)
{
    initializeVariantGeometry();
}

PieceVariant::PieceVariant(Clipper2Lib::Path64 initalPath)
    : Shape(initalPath)
{
    initializeVariantGeometry();
}

const int64_t PieceVariant::getGlobalId(bool isFlipped) const
{
    bool flipped = isFlipped;
    if (!m_isTwoWay) {
        flipped = false;
    }

    return static_cast<int64_t>(
        (static_cast<uint64_t>(flipped) << 63) |
        ((m_pieceId & 0x7FFFFFFFFFFFFULL) << 12) |
        (m_variantId & 0xFFFULL)
    );
}

const std::vector<geometry::Point64> &PieceVariant::getBoundary(bool isFlipped) const
{
    if (m_isTwoWay && isFlipped) {
        return m_flippedBoundary;
    }
    return m_boundary;
}

const Clipper2Lib::Path64 &PieceVariant::getPath(bool isFlipped) const
{
    if (m_isTwoWay && isFlipped) {
        return m_flippedShapePath;
    }
    return m_shapePath;
}

const Clipper2Lib::Path64 PieceVariant::getPathAt(int64_t x, int64_t y, bool isFlipped) const
{
    Clipper2Lib::Path64 tranlated_path;

    tranlated_path.reserve(m_shapePath.size());

    if (m_isTwoWay && isFlipped) {
        for (const auto& point : m_flippedShapePath) {
            tranlated_path.push_back(Clipper2Lib::Point64(point.x + x, point.y + y));
        }
    } else {
        for (const auto& point : m_shapePath) {
            tranlated_path.push_back(Clipper2Lib::Point64(point.x + x, point.y + y));
        }
    }
    return tranlated_path;
}

const std::vector<Clipper2Lib::Path64> &PieceVariant::getDecomposedShape(bool isFlipped) const
{
    if (m_isTwoWay && isFlipped) {
        return m_decomposedFlippedShape;
    }
    return m_decomposedShape;
}

std::string PieceVariant::toString() const
{
    std::string boundaryStr = std::accumulate(
        std::next(m_boundary.begin()), m_boundary.end(),
        m_boundary.front().toString(),
        [](std::string a, geometry::Point64 b) {
            return a + ", " + b.toString();
        }
    );

    return std::format("rotationAngle: {}, isMirrored: {}, requiredQuantity: {}, boundary: ({})",
                        m_rotationAngle,
                        m_isMirrored,
                        m_requiredQuantity,
                        boundaryStr);
}

} // namespace pmg::pattern