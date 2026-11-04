# pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "nesting/nesting_result.hpp"
#include "geometry/geometry_types.hpp"

namespace pmg::pattern {
    class PatternPiece;
    class Fabric;
} // namespace pmg::pattern

namespace pmg::nesting
{
    class BasicAlgorithm;
} // namespace pmg::nesting


class NestingEngineFacade {
private:
    std::unique_ptr<pmg::nesting::BasicAlgorithm> m_engine;
public:
    NestingEngineFacade(int64_t width, int64_t length);
    NestingEngineFacade(const pmg::pattern::Fabric& fabric);
    ~NestingEngineFacade();

    pmg::nesting::NestingResult execute(std::vector<pmg::pattern::PatternPiece>& patternPieces);
};
