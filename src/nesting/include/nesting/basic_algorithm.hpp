#pragma once

#include <vector>

#include "nesting/algorithm_base.hpp"

namespace pmg::nesting {

class BasicAlgorithm : public AlgorithmBase {
protected:
    NestingResult runAlgorithm() override;
public:
    using AlgorithmBase::AlgorithmBase;
};

} // namespace pmg::nesting