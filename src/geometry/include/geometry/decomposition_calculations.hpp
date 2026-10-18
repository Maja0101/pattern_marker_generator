#pragma once 

#include <vector>

#include <mapbox/earcut.hpp>

#include "geometry/geometry_types.hpp"

namespace mapbox {
namespace util {
    template <>
    struct nth<0, pmg::geometry::Point64> {
        inline static auto get(const pmg::geometry::Point64& p) { return p.x; }
    };
    
    template <>
    struct nth<1, pmg::geometry::Point64> {
        inline static auto get(const pmg::geometry::Point64& p) { return p.y; }
    };
}
}

namespace pmg::geometry::calculations {

std::vector<std::vector<Point64>> hertelMehlhornAlgorithm(const std::vector<Point64>& polygon);

} // namespace pmg::geometry::calculations