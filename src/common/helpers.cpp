#include "common/helpers.hpp"

namespace pmg::geometry {

int64_t crossProduct(const Point64 &a, const Point64 &b, const Point64 &c)
{
    return ((b.x - a.x) * (c.y - b.y)) - ((b.y - a.y) * (c.x - b.x));
}

Clipper2Lib::Path64 toClipperPath(const std::vector<Point64> &polygon)
{
    Clipper2Lib::Path64 path;
    path.reserve(polygon.size());
    for (const auto& point : polygon) {
        path.emplace_back(point.x, point.y);
    }
    return path;
}

std::vector<Point64> fromClipperPath(const Clipper2Lib::Path64 &path)
{
    std::vector<Point64> polygon;
    polygon.reserve(path.size());
    for (const auto& point : path) {
        polygon.push_back({point.x, point.y});
    }
    return polygon;
}

inline const int DEGREES_0 = 0;
inline const int DEGREES_90 = 90;
inline const int DEGREES_180 = 180;
inline const int DEGREES_270 = 270;
inline const int DEGREES_360 = 360;
    
constexpr double DEG_TO_RAD = std::numbers::pi / 180.0;

std::vector<Point64> transformPolygon(const std::vector<Point64> &polygon, int rotationAngle, bool isMirrored, bool toBeAnchored, bool mirroringInFlippedXAxis) noexcept
{
    if (polygon.empty()) return {};

    // Normalize rotation angle
    rotationAngle %= DEGREES_360;
    if (rotationAngle < 0) rotationAngle += DEGREES_360;

    // allocate the transformed polygon
    const size_t SIZE = polygon.size();
    std::vector<Point64> transformed(SIZE);

    // set minX, minY values 
    // in order to anchor the transformed points back 
    // to 0,0 at bottom left
    int64_t minX = std::numeric_limits<int64_t>::max();
    int64_t minY = std::numeric_limits<int64_t>::max();

    if (rotationAngle == DEGREES_0 || rotationAngle == DEGREES_90 || rotationAngle == DEGREES_180 || rotationAngle == DEGREES_270) {
        for (size_t i = 0; i < SIZE; ++i) {
            int64_t x = polygon[i].x;
            int64_t y = polygon[i].y;
            int64_t rx;
            int64_t ry;
            
            switch (rotationAngle)
            {
                case DEGREES_90:  rx =  y; ry = -x; break;
                case DEGREES_180: rx = -x; ry = -y; break;
                case DEGREES_270: rx = -y; ry =  x; break;
                default:  rx =  x; ry =  y; break;
            }

            if (mirroringInFlippedXAxis) {
                ry = isMirrored ? -ry : ry;
            } else {
                rx = isMirrored ? -rx : rx;
            }

            transformed[i].x = rx;
            transformed[i].y = ry;

            minX = std::min(rx, minX);
            minY = std::min(ry, minY);
        }
    } else {
        const double RADIANS = static_cast<double>(rotationAngle) * DEG_TO_RAD;
        const double COS_A = std::cos(RADIANS);
        const double SIN_A = std::sin(RADIANS);

        for (size_t i = 0; i < SIZE; ++i) {
            auto x = static_cast<double>(polygon[i].x);
            auto y = static_cast<double>(polygon[i].y);

            auto rx = static_cast<int64_t>(std::llround((x * COS_A) + (y * SIN_A)));
            auto ry = static_cast<int64_t>(std::llround((-x * SIN_A) + (y * COS_A)));

            if (mirroringInFlippedXAxis) {
                ry = isMirrored ? -ry : ry;
            } else {
                rx = isMirrored ? -rx : rx;
            }

            transformed[i].x = rx;
            transformed[i].y = ry;

            minX = std::min(rx, minX);
            minY = std::min(ry, minY);
        }
    }

    if (toBeAnchored) {
        for (size_t i = 0; i < SIZE; ++i) {
            transformed[i].x -= minX;
            transformed[i].y -= minY;
        }
    }

    if (isMirrored) {
        std::ranges::reverse(transformed);
    }

    return transformed;
}

} // namespace pmg::geometry
