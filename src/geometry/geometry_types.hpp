#pragma once

#include <cstdint>
#include <algorithm>
#include <format>
#include <string>

namespace pmg::geometry {

struct Point64 {
    int64_t x{0};
    int64_t y{0};

    std::string toString() const {
        return std::format("({},{})", x, y);
    }
};

// bounding box, etc.

} // namespace pmg:geometry