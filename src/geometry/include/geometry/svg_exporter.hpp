#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace pmg::pattern {
    class NestedItem;
    class Fabric;
}

namespace pmg::geometry {

class SvgExporter {
public:
    static bool exportLayout(const std::string& filepath, 
                             const pattern::Fabric& fabric,
                             const std::vector<pattern::NestedItem>& items,
                             int64_t includedSeamAllowance, 
                             int64_t includedCuttingAllowance); 
};

} // namespace pmg::geometry