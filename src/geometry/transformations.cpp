#include "geometry/transformations.hpp"

namespace pmg::geometry {
    
Point64 transformPoint(Point64 point, int rotationAngle, bool isMirrored) noexcept {
    if (isMirrored) point.x = -point.x;

    switch (rotationAngle % 360) {
        case 90:  return {-point.y,  point.x};
        case 180: return {-point.x, -point.y};
        case 270: return { point.y, -point.x};
        default:  return { point.x,  point.y}; 
    }
}

} // namespace pmg::geometry
