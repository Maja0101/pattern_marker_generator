#pragma once

#include <vector>

#include "common/geometry_types.hpp"
#include "pattern/variant_constraint.hpp"

namespace pmg::bench {

std::vector<int64_t> getBenchIds();
std::vector<std::vector<geometry::Point64>> getBenchShapes();
std::vector<std::vector<pattern::VariantConstraint>> getBenchConstraints();

} // namespace pmg::bench