#include <cmath>
#include <vector>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <numbers>

#include "geometry/transformations.hpp"

namespace pmg::geometry::calculations {
    
constexpr double DEG_TO_RAD = std::numbers::pi / 180.0;

std::vector<Point64> transformPolygon(const std::vector<Point64> &polygon, int rotationAngle, bool isMirrored) noexcept
{
    if (polygon.empty()) return {};

    // Normalize rotation angle
    rotationAngle %= 360;
    if (rotationAngle < 0) rotationAngle += 360;

    // allocate the transformed polygon
    const size_t size = polygon.size();
    std::vector<Point64> transformed(size);

    // set minX, minY values 
    // in order to anchor the transformed points back 
    // to 0,0 at bottom left
    int64_t minX = std::numeric_limits<int64_t>::max();
    int64_t minY = std::numeric_limits<int64_t>::max();

    if (rotationAngle == 0 || rotationAngle == 90 || rotationAngle == 180 || rotationAngle == 270) {
        for (size_t i = 0; i < size; ++i) {
            int64_t x = polygon[i].x;
            int64_t y = isMirrored ? -polygon[i].y : polygon[i].y;
            int64_t rx, ry;

            switch (rotationAngle)
            {
                case 90:  rx =  y; ry = -x; break;
                case 180: rx = -x; ry = -y; break;
                case 270: rx = -y; ry =  x; break;
                default:  rx =  x; ry =  y; break;
            }

            transformed[i].x = rx;
            transformed[i].y = ry;

            if (rx < minX) minX = rx;
            if (ry < minY) minY = ry;
        }
    } else {
        const double radians = static_cast<double>(rotationAngle) * DEG_TO_RAD;
        const double cosA = std::cos(radians);
        const double sinA = std::sin(radians);

        for (size_t i = 0; i < size; ++i) {
            double x = static_cast<double>(polygon[i].x);
            double y = static_cast<double>(isMirrored ? -polygon[i].y : polygon[i].y);

            int64_t rx = static_cast<int64_t>(std::llround(x * cosA + y * sinA));
            int64_t ry = static_cast<int64_t>(std::llround(-x * sinA + y * cosA));

            transformed[i].x = rx;
            transformed[i].y = ry;

            if (rx < minX) minX = rx;
            if (ry < minY) minY = ry;
        }
    }

    for (size_t i = 0; i < size; ++i) {
        transformed[i].x -= minX;
        transformed[i].y -= minY;
    }

    if (isMirrored) {
        std::reverse(transformed.begin(), transformed.end());
    }

    return transformed;
}

std::vector<Point64> rotateBy180(const std::vector<Point64> &polygon) noexcept
{
    return transformPolygon(polygon, 180, false);
}

} // namespace pmg::geometry::calculations
