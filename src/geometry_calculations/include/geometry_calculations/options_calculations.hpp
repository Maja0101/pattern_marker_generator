#pragma once

#include <vector>

#include "pattern/nested_item.hpp"
#include "pattern/nesting_result.hpp"

namespace pmg::geometry::calculations::utils {

struct ScoringWeights {
    double wFabric{0.5};
    double wScrap{0.25};
    double wWaste{0.20};
    double wFrag{0.05};
    double alphaFabric{0.6};
};

} // namespace pmg::geometry::calculations::utils

namespace pmg::geometry::calculations {

bool numberOfOptionsWithinLimit(const std::vector<pattern::NestedItem>& items, uint64_t limit = 100000);
double calculateNestingScore(const pattern::NestingStats& stats, const utils::ScoringWeights& w = utils::ScoringWeights());

} // namespace pmg::geometry::calculations 