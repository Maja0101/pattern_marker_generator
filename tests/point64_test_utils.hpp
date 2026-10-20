#pragma once

#include <vector>
#include <ostream>

#include "geometry/geometry_types.hpp"

namespace pmg::geometry {

bool pointsSimiliar(const Point64& a, const Point64& b, int64_t maxDelta = 1) {
    return std::abs(a.x - b.x) <= maxDelta && std::abs(a.y - b.y) <= maxDelta;
}

bool polygonsSimilar(const std::vector<Point64>& actual, 
                  const std::vector<Point64>& expected, 
                  int64_t maxDelta = 1) {
    if (actual.size() != expected.size()) return false;
    for (size_t i = 0; i < actual.size(); ++i) {
        if (!pointsSimiliar(actual[i], expected[i], maxDelta)) return false;
    }
    return true;
}

inline std::ostream& operator<<(std::ostream& os, const Point64& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

} // namespace pmg::geometry    