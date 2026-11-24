# pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "pattern/nesting_result.hpp"

namespace pmg::pattern {
    class PatternPiece;
    class Fabric;
} // namespace pmg::pattern

namespace pmg::nesting {
    class AlgorithmBase;
} // namespace pmg::nesting

namespace pmg::api {

class NestingEngineFacade {
private:
    std::unique_ptr<pmg::nesting::AlgorithmBase> m_engine;
public:
    NestingEngineFacade(const pmg::pattern::Fabric& fabric, int64_t algId);
    ~NestingEngineFacade();

    pmg::pattern::NestingResult execute(std::vector<pmg::pattern::PatternPiece>& patternPieces);
    int64_t calcMinimum(std::vector<pmg::pattern::PatternPiece>& patternPieces);
};

pmg::pattern::NestingStats calculateStatsFor(
    const pmg::pattern::Fabric& fabric,
    std::vector<pmg::pattern::PatternPiece>& patternPieces,
    const std::vector<pmg::pattern::ItemPlacement>& itemPlacements
);

} // namespace pmg::api
