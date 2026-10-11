#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace pmg::nesting {
    class NestedItem;
}

namespace pmg::geometry {

class SvgExporter {
public:
    static bool exportLayout(const std::string& filepath, 
                             int64_t fabricWidth,
                             int64_t fabricHeight,
                             const std::vector<nesting::NestedItem>& items); 
};

} // namespace pmg::geometry