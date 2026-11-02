#pragma once

#include <vector>

#include "nesting/algorithm_base.hpp"

namespace pmg::nesting {

class AdvancedAlgorithm : public AlgorithmBase {
private:
    void evaluateDifferentRotations(std::vector<nesting::NestedItem>& items, size_t index, NestingScore& best);
    int m_svgExport;
protected:
    NestingResult runAlgorithm() override;
public:
    using AlgorithmBase::AlgorithmBase;
};

} // namespace pmg::nesting