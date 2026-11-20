#pragma once

#include "nesting/algorithm_base.hpp"

namespace pmg::nesting {

class BasicAlgorithm : public AlgorithmBase {
private:
    void sortItems();
protected:
    NestingResult runAlgorithm() override;
    void prepareNestingItems(std::vector<pattern::PatternPiece>& patternPieces) override;
public:
    using AlgorithmBase::AlgorithmBase;
};

} // namespace pmg::nesting