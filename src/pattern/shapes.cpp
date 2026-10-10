#include <format>
#include <numeric>
#include <ranges>

#include "pattern/shapes.hpp"

namespace pmg::pattern {

Shape::Shape(std::vector<geometry::Point64> initalBoundary)
    : m_boundary(std::move(initalBoundary)) {
        m_shape_path = toClipperPath();
        m_shape_bounding_box = Clipper2Lib::GetBounds(m_shape_path);
    }

Clipper2Lib::Path64 Shape::toClipperPath() const
{
    Clipper2Lib::Path64 path;
    path.reserve(m_boundary.size());
    for (const auto& point : m_boundary) {
        path.push_back({point.x, point.y});
    }
    return path;
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

PieceVariant::PieceVariant(int rotationAngle, bool isMirrored, int requiredQuantity, std::vector<geometry::Point64> initalBoundary)
    : Shape(initalBoundary), m_rotationAngle(rotationAngle), m_isMirrored(isMirrored), m_requiredQuantity(requiredQuantity) {}

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