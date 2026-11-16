#pragma once

namespace pmg::timing {

struct PhaseTimings {
    double preparationNs = 0.0;
    double calculationNs = 0.0;
    double totalNs = 0.0;
};

} // namespace pmg::timing