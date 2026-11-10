#include "api/nesting_api.hpp"
#include "nesting/algorithm_base.hpp"
#include "nesting/basic_algorithm.hpp"
#include "nesting/advanced_algorithm.hpp"
#include "pattern/shapes.hpp"
#include "pattern/pattern_piece.hpp"

// NestingEngineFacade::NestingEngineFacade(int64_t width, int64_t length, int64_t algId)
//     : m_engine([&]() -> std::unique_ptr<pmg::nesting::AlgorithmBase> {
//         if (algId == 1) {
//             return std::make_unique<pmg::nesting::BasicAlgorithm>(width, length);
//         }
//         return std::make_unique<pmg::nesting::AdvancedAlgorithm>(width, length);
//       }()) {}

NestingEngineFacade::NestingEngineFacade(const pmg::pattern::Fabric &fabric, int64_t algId)
    : m_engine([&]() -> std::unique_ptr<pmg::nesting::AlgorithmBase> {
        if (algId == 1) {
            return std::make_unique<pmg::nesting::BasicAlgorithm>(fabric);
        }
        return std::make_unique<pmg::nesting::AdvancedAlgorithm>(fabric);
      }()) {}

NestingEngineFacade::~NestingEngineFacade() = default;

pmg::nesting::NestingResult NestingEngineFacade::execute(std::vector<pmg::pattern::PatternPiece> &patternPieces)
{
    return m_engine->execute(patternPieces);
}
