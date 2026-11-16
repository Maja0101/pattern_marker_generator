#pragma once

#include <vector>

#include "nesting/algorithm_base.hpp"

namespace pmg::nesting::utils {

enum class MoveType : uint8_t {
    Flip,
    Swap,
    Shift
};

struct Move {
    MoveType type;
    size_t i;
    size_t j;
};

} // namespace pmg::nesting::utils

namespace pmg::nesting {

class AdvancedAlgorithm : public AlgorithmBase {
private:
    int m_svgExport;
    void evaluateDifferentRotations(std::vector<nesting::NestedItem>& items, size_t index, NestingScore& best);
    void applyMove(const utils::Move& m);
    void undoMove(const utils::Move& m);
    NestingScore fullSearch();
    NestingScore checkOneOption();
    NestingScore simulatedAnnealing();
protected:
    NestingResult runAlgorithm() override;
public:
    using AlgorithmBase::AlgorithmBase;
};

} // namespace pmg::nesting