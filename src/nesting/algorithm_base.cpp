#include <chrono>

#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "utils/timer.hpp"
#include "geometry_calculations/calculations.hpp"
#include "geometry_calculations/nfp_ifp_calculations.hpp"
#include "nesting/bottom_left.hpp"
#include "nesting/algorithm_base.hpp"

namespace pmg::nesting {
void AlgorithmBase::reset()
{
    m_ifpCache.clear();
    m_nfpCache.clear();
    m_originalItems.clear();
}

pattern::NestingResult AlgorithmBase::runSingleBottomLeft(std::vector<pattern::NestedItem> &items)
{
    BottomLeft bottomLeft(m_fabric, m_ifpCache, m_nfpCache);

    return bottomLeft.performNesting(items);
}

void AlgorithmBase::prepareNestingItems(std::vector<pattern::PatternPiece> &patternPieces)
{
    if constexpr (config::DEVELOPMENT) SPDLOG_INFO("Preparing the geometry...");
        
    timing::Timer timer;

    m_originalItems = geometry::calculations::prepareItems(patternPieces);

    std::vector<pmg::pattern::PieceVariant> allPossibleVariants = geometry::calculations::getAllVariants(patternPieces);

    m_ifpCache = geometry::calculations::calculateIFPCache(m_fabric, allPossibleVariants);
    m_nfpCache = geometry::calculations::calculateNFPCache(allPossibleVariants);

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("ifp cache: {}, nfp cache: {}", m_ifpCache.size(), m_nfpCache.size()));
        for (const auto& [key, val] : m_ifpCache) {
            SPDLOG_INFO(std::format("ifp cache id {}", key));
        }
        for (const auto& [key, val] : m_nfpCache) {
            SPDLOG_INFO(std::format("nfp cache id {} {}", key.variantIdA, key.variantIdB));
        }
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& item : m_originalItems) {
            SPDLOG_INFO(item.toString());
        }
    }
}

AlgorithmBase::AlgorithmBase(int64_t width, int64_t length)
    : m_fabric(width, length) {}

AlgorithmBase::AlgorithmBase(pattern::Fabric fabric)
    : m_fabric(std::move(fabric)) {}

pattern::NestingResult AlgorithmBase::execute(std::vector<pattern::PatternPiece> &patternPieces, timing::PhaseTimings* timings)
{
    reset();
    
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

    // Time points for benchmarking
    std::chrono::steady_clock::time_point t0;
    std::chrono::steady_clock::time_point t1;
    std::chrono::steady_clock::time_point t2;

    if constexpr (config::BENCHMARK) t0 = std::chrono::steady_clock::now();

    prepareNestingItems(patternPieces);

    if constexpr (config::BENCHMARK) t1 = std::chrono::steady_clock::now();

    pattern::NestingResult result;
    {
        if constexpr (config::DEVELOPMENT) SPDLOG_INFO("Nesting...");
        timing::Timer timer;
        result = runAlgorithm();
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
    int executionTimeMs = std::chrono::duration_cast<std::chrono::duration<int, std::milli>>(end - start).count();    

    if constexpr (config::DEVELOPMENT) {
        SPDLOG_INFO(std::format("Sum execution time: {} ms", executionTimeMs));
    }

    result.executionTime = executionTimeMs;

    if constexpr (config::BENCHMARK) {
        t2 = std::chrono::steady_clock::now();
        timings->preparationNs = std::chrono::duration<double, std::nano>(t1 - t0).count();
        timings->calculationNs = std::chrono::duration<double, std::nano>(t2 - t1).count();
        timings->totalNs = std::chrono::duration<double, std::nano>(t2 - t0).count();
    }

    return result;
}

} // namespace pmg::nesting