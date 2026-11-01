#include "api/nesting_api.hpp"
#include "nesting/basic_algorithm.hpp"
#include "pattern/shapes.hpp"
#include "pattern/pattern_piece.hpp"

NestingEngineFacade::NestingEngineFacade(int64_t width, int64_t length)
    : m_engine(std::make_unique<pmg::nesting::BasicAlgorithm>(width, length)) {}

NestingEngineFacade::NestingEngineFacade(const pmg::pattern::Fabric &fabric)
    : m_engine(std::make_unique<pmg::nesting::BasicAlgorithm>(fabric)) {}

NestingEngineFacade::~NestingEngineFacade() = default;

pmg::nesting::NestingResult NestingEngineFacade::execute(std::vector<pmg::pattern::PatternPiece> &patternPieces)
{
    return m_engine->execute(patternPieces);
}
