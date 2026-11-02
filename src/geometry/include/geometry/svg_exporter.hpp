#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace pmg::nesting {
    class NestedItem;
}

namespace pmg::pattern {
    class Fabric;
}

namespace pmg::geometry {

class SvgExporter {
public:
    static bool exportLayout(const std::string& filepath, 
                             const pattern::Fabric& fabric,
                             const std::vector<nesting::NestedItem>& items,
                             int64_t includedSeamAllowance, 
                             int64_t includedCuttingAllowance); 
};

} // namespace pmg::geometry