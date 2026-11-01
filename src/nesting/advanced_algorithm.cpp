#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "utils/timer.hpp"
#include "geometry/svg_exporter.hpp"
#include "nesting/advanced_algorithm.hpp"
#include "geometry/options_calculations.hpp"

namespace pmg::nesting {

NestingResult AdvancedAlgorithm::runAlgorithm()
{
    // Sort by size

    if(geometry::calculations::numberOfOptionsWithinLimit(m_originalItems)) {
        // Generate every permutation

        // For each order check every rotation by 180 combination

        // return the best result
    } else {
        // Rotate certain elements (the pieces that are doubled and/or mirrored)

        // Run once and return the result
    }
    return NestingResult();
}

} // namespace pmg::nesting