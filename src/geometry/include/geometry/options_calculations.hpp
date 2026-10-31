#pragma once

#include <vector>

#include <clipper2/clipper.h>

#include "nesting/nested_item.hpp"
#include "pattern/shapes.hpp"

namespace pmg::geometry::calculations {

bool numberOfOptionsWithinLimit(const std::vector<nesting::NestedItem>& items, uint64_t limit = 4000);

} // namespace pmg::geometry::calculations 