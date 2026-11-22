#pragma once

#include <cstdint>
#include <format>
#include <string>

namespace pmg::pattern {

struct Point64 {
    int64_t x{0};
    int64_t y{0};

    auto operator<=>(const Point64&) const = default;

    [[nodiscard]] std::string toString() const {
        return std::format("({},{})", x, y);
    }
};

} // namespace pmg:pattern