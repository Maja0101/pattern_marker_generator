#pragma once

#include <vector>

#include "nesting/algorithm_base.hpp"

namespace pmg::nesting {

class AdvancedAlgorithm : public AlgorithmBase {
private:
    int m_svgExport;
    void evaluateDifferentRotations(std::vector<nesting::NestedItem>& items, size_t index, NestingScore& best);
protected:
    NestingResult runAlgorithm() override;
public:
    using AlgorithmBase::AlgorithmBase;
};

} // namespace pmg::nesting