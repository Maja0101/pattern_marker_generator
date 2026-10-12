#pragma once

#include <vector>

#include "nesting/algorithm_base.hpp"

namespace pmg::nesting {

class BasicAlgorithm : public AlgorithmBase {
protected:
    bool runAlgorithm() override;
public:
    using AlgorithmBase::AlgorithmBase;
};

} // namespace pmg::nesting