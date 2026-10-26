#include <format>
#include <numeric>
#include <ranges>

#include <spdlog/spdlog.h>

#include "pattern/shapes.hpp"
#include "geometry/decomposition_calculations.hpp"
#include "geometry/calculations.hpp"
#include "utils/config.hpp"

namespace pmg::pattern {

Shape::Shape(std::vector<geometry::Point64> initalBoundary)
    : m_boundary(std::move(initalBoundary)) {
        m_shape_path = geometry::calculations::toClipperPath(m_boundary);
        m_shape_bounding_box = Clipper2Lib::GetBounds(m_shape_path);
    }

Shape::Shape(int64_t width, int64_t length)
{
    constexpr int64_t maxValue = 100000001;
    width = (width < 0) ? 0 : (width % maxValue);
    length = (length < 0) ? 0 : (length % maxValue);
    m_boundary = {
        {0, 0},
        {length, 0},
        {length, width},
        {0, width}
    };
    m_shape_path = geometry::calculations::toClipperPath(m_boundary);
    m_shape_bounding_box = Clipper2Lib::GetBounds(m_shape_path);
}

Shape::Shape(Clipper2Lib::Path64 initalPath)
    : m_shape_path(std::move(initalPath))
{
    m_boundary = geometry::calculations::fromClipperPath(m_shape_path);
    m_shape_bounding_box = Clipper2Lib::GetBounds(m_shape_path);
}

const Clipper2Lib::Path64 Shape::getPathAt(int64_t x, int64_t y) const
{
    Clipper2Lib::Path64 tranlated_path;

    tranlated_path.reserve(m_shape_path.size());

    for (const auto& point : m_shape_path) {
        tranlated_path.push_back(Clipper2Lib::Point64(point.x + x, point.y + y));
    }
    return tranlated_path;
}

const Clipper2Lib::Rect64 Shape::getBoundingBoxAt(int64_t x, int64_t y) const
{
    Clipper2Lib::Rect64 tranlated_box = m_shape_bounding_box;

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

    if (m_isConvex) {
        m_decomposedShape = { m_shape_path };
    } else {
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("decomposing {}", this->toString()));
        }
        std::vector<std::vector<pmg::geometry::Point64>> subShapes = geometry::calculations::hertelMehlhornAlgorithm(m_boundary);

        for (const auto& subShape : subShapes) {
            m_decomposedShape.push_back(geometry::calculations::toClipperPath(subShape));
        }
    }
}

PieceVariant::PieceVariant(int64_t globalId, int id, int rotationAngle, bool isMirrored, int requiredQuantity, int64_t seamAllowance, int64_t cuttingAllowance, std::vector<geometry::Point64> initalBoundary)
    : Shape(initalBoundary), m_globalId(globalId), m_id(id), m_rotationAngle(rotationAngle), m_isMirrored(isMirrored), m_requiredQuantity(requiredQuantity), m_seamAllowance(seamAllowance), m_cuttingAllowance(cuttingAllowance)
{
    initializeVariantGeometry();
}

PieceVariant::PieceVariant(Clipper2Lib::Path64 initalPath)
    : Shape(initalPath)
{
    initializeVariantGeometry();
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