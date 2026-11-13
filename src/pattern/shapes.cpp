#include <format>
#include <numeric>

#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "geometry/decomposition_calculations.hpp"
#include "geometry/calculations.hpp"
#include "geometry/transformations.hpp"
#include "pattern/shapes.hpp"

namespace pmg::pattern {
inline constexpr int64_t SCALE = 1000;

Shape::Shape(const std::vector<geometry::Point64> &initalBoundary, bool toBeScaled)
    : m_shapePath([&initalBoundary, toBeScaled]() {
        if (toBeScaled) {
            std::vector<pmg::geometry::Point64> boundary;
            boundary.reserve(initalBoundary.size());
            for (const auto& point : initalBoundary) {
                boundary.push_back({point.x * SCALE, point.y * SCALE});
            }
            return Clipper2Lib::SimplifyPath(geometry::calculations::toClipperPath(boundary), 0.0);
        }
        return Clipper2Lib::SimplifyPath(geometry::calculations::toClipperPath(initalBoundary), 0.0);
      }()), 
      m_boundary(geometry::calculations::fromClipperPath(m_shapePath)),
      m_shapeBoundingBox(Clipper2Lib::GetBounds(m_shapePath)) {}

Shape::Shape(int64_t width, int64_t length, bool toBeScaled)
    : m_shapePath([width, length, toBeScaled]() {
        constexpr int64_t MAX_VALUE = 100000001;
        int64_t scale = toBeScaled ? SCALE : 1;
        int64_t w = (width < 0) ? 0 : ((width * scale) % MAX_VALUE);
        int64_t l = (length < 0) ? 0 : ((length * scale) % MAX_VALUE);
        Clipper2Lib::Path64 temp;
        temp.reserve(4);
        temp.emplace_back(0L, 0L);
        temp.emplace_back(l, 0L);
        temp.emplace_back(l, w);
        temp.emplace_back(0L, w);
        return temp;
      }()), 
      m_boundary(geometry::calculations::fromClipperPath(m_shapePath)),
      m_shapeBoundingBox(Clipper2Lib::GetBounds(m_shapePath)) {}

Shape::Shape(const Clipper2Lib::Path64 &initalPath)
    : m_shapePath(Clipper2Lib::SimplifyPath(initalPath, 0.0)),
      m_boundary(geometry::calculations::fromClipperPath(m_shapePath)),
      m_shapeBoundingBox(Clipper2Lib::GetBounds(m_shapePath)) {}

Clipper2Lib::Path64 Shape::getPathAt(int64_t x, int64_t y) const
{
    Clipper2Lib::Path64 tranlatedPath;

    tranlatedPath.reserve(m_shapePath.size());

    for (const auto& point : m_shapePath) {
        tranlatedPath.emplace_back(point.x + x, point.y + y);
    }
    return tranlatedPath;
}

Clipper2Lib::Rect64 Shape::getBoundingBoxAt(int64_t x, int64_t y) const
{
    Clipper2Lib::Rect64 tranlatedBox = m_shapeBoundingBox;

    tranlatedBox.left   += x;
    tranlatedBox.right  += x;
    tranlatedBox.top    += y;
    tranlatedBox.bottom += y;

    return tranlatedBox;
}

std::string Shape::toString() const
{
    std::string boundaryStr = std::accumulate(
        std::next(m_boundary.begin()), m_boundary.end(),
        m_boundary.front().toString(),
        [](std::string const& a, geometry::Point64 b) {
            return a + ", " + b.toString();
        }
    );

    return std::format("boundary: ({})", boundaryStr);
}

Fabric::InitState Fabric::transformBoundary(const std::vector<geometry::Point64> &initalBoundary, int grainline, bool toBeScaled) {
    InitState state;
    
    const int DEGREES_360 = 360;
    state.grainline = grainline % DEGREES_360;
    if (state.grainline < 0) state.grainline += DEGREES_360;

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& p : initalBoundary) {
            SPDLOG_INFO(std::format("original boundary point {}", p.toString()));
        }
    }

    state.boundary = geometry::calculations::fromClipperPath(
        Clipper2Lib::SimplifyPath(
            geometry::calculations::toClipperPath(initalBoundary), 
            0.1)
        );

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& p : state.boundary) {
            SPDLOG_INFO(std::format("simplified polygon point {}", p.toString()));
        }
    }

    if (toBeScaled) {
        for (auto& point : state.boundary) {
            point.x *= SCALE;
            point.y *= SCALE;
        }
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& p : state.boundary) {
            SPDLOG_INFO(std::format("scaled polygon point {}", p.toString()));
        }
    }

    state.boundary = geometry::calculations::transformPolygon(state.boundary, state.grainline, false, false);

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& p : state.boundary) {
            SPDLOG_INFO(std::format("rotated by grainline polygon point {}", p.toString()));
        }
    }

    state.referencePoint = geometry::calculations::getTopLeftCorner(state.boundary);

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("reference point {}", state.referencePoint.toString()));
    }

    const int CCW_90 = 270;
    state.boundary = geometry::calculations::transformPolygon(state.boundary, CCW_90, false, true);

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& p : state.boundary) {
            SPDLOG_INFO(std::format("rotated by 90 ccw polygon point {}", p.toString()));
        }
    }

    return state;
}

Fabric::Fabric(InitState &&state) 
    : Shape(state.boundary), m_grainline(state.grainline), m_referencePoint(std::move(state.referencePoint)) {}

Fabric::Fabric(const std::vector<geometry::Point64> &initalBoundary, int grainline, bool toBeScaled)
    : Fabric(transformBoundary(initalBoundary, grainline, toBeScaled)) {}

Fabric::Fabric(int64_t width, int64_t length, bool toBeScaled)
    : Shape(width, length, toBeScaled), m_grainline(0), 
      m_referencePoint(geometry::Point64{m_shapeBoundingBox.top, m_shapeBoundingBox.right}) {}

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
            SPDLOG_INFO("decomposing");
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

PieceVariant::PieceVariant(int variantId, int64_t pieceId, 
    int rotationAngle, bool isMirrored, int requiredQuantity, bool twoWayAllowed, 
    int64_t seamAllowance, int64_t cuttingAllowance, 
    const std::vector<geometry::Point64> &initalBoundary, geometry::Point64 referencePoint)
    : Shape(initalBoundary), m_pieceId(pieceId), m_variantId(variantId), 
      m_rotationAngle(rotationAngle), m_isMirrored(isMirrored), 
      m_requiredQuantity(requiredQuantity), m_isTwoWay(twoWayAllowed),
      m_seamAllowance(seamAllowance), m_cuttingAllowance(cuttingAllowance),
      m_referencePoint(referencePoint)
{
    initializeVariantGeometry();
}

PieceVariant::PieceVariant(const Clipper2Lib::Path64 &initalPath)
    : Shape(initalPath)
{
    initializeVariantGeometry();
}

int64_t PieceVariant::getGlobalId(bool isFlipped) const
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

const std::vector<geometry::Point64> &PieceVariant::getBoundaryForVariant(bool isFlipped) const
{
    if (m_isTwoWay && isFlipped) {
        return m_flippedBoundary;
    }
    return m_boundary;
}

const Clipper2Lib::Path64 &PieceVariant::getPathForVariant(bool isFlipped) const
{
    if (m_isTwoWay && isFlipped) {
        return m_flippedShapePath;
    }
    return m_shapePath;
}

Clipper2Lib::Path64 PieceVariant::getPathForVariantAt(int64_t x, int64_t y, bool isFlipped) const
{
    Clipper2Lib::Path64 tranlatedPath;

    tranlatedPath.reserve(m_shapePath.size());

    if (m_isTwoWay && isFlipped) {
        for (const auto& point : m_flippedShapePath) {
            tranlatedPath.emplace_back(point.x + x, point.y + y);
        }
    } else {
        for (const auto& point : m_shapePath) {
            tranlatedPath.emplace_back(point.x + x, point.y + y);
        }
    }
    return tranlatedPath;
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
        [](std::string const& a, geometry::Point64 b) {
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