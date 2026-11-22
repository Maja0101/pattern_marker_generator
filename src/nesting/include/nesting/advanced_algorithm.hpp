#pragma once

#include <vector>
#include <algorithm>
#include <random>

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

    std::mt19937 m_rng;
    std::uniform_real_distribution<double> m_probDist;
    std::uniform_int_distribution<size_t> m_idxDist;

    void evaluateDifferentRotations(std::vector<pattern::NestedItem>& items, size_t index, pattern::NestingScore& best);
    void applyMove(const utils::Move& m);
    void undoMove(const utils::Move& m);

    pattern::NestingScore fullSearch();
    pattern::NestingScore checkOneOption();

    pattern::NestingScore evaluate();
    void resetAllPositions();
    bool checkAllIdentical();
    size_t pickBiasedIndex(size_t n, double exponent);
    bool pickValidMove(size_t n, bool identical, utils::Move& outMove, double biasExponent);
    double setInitialTemp(const pattern::NestingScore& currentScore, pattern::NestingScore& bestScore, std::vector<pattern::NestedItem>& bestItems, size_t n, bool identical);
    pattern::NestingScore simulatedAnnealing();
protected:
    pattern::NestingResult runAlgorithm() override;
public:
    explicit AdvancedAlgorithm(int64_t width, int64_t length);
    explicit AdvancedAlgorithm(pattern::Fabric fabric);
};

} // namespace pmg::nesting