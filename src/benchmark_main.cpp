#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <iostream>
#include <limits>
#include <random>
#include <span>
#include <stdexcept>
#include <vector>

#include "utils/timer.hpp"
#include "common/geometry_types.hpp"
#include "pattern/pattern_piece.hpp"
#include "pattern/shapes.hpp"
#include "pattern/nested_item.hpp"
#include "nesting/basic_algorithm.hpp"
#include "nesting/advanced_algorithm.hpp"
#include "pattern/nesting_result.hpp"
#include "sample_input.hpp"
#include "utils_benchmark/benchmark_conf.hpp"

namespace pmg::bench {

template <typename T>
inline void doNotOptimize(T const& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

struct MetricStats {
    double sumNs = 0.0;
    double minNs = std::numeric_limits<double>::max();

    void record(double ns) {
        sumNs += ns;
        minNs = std::min(minNs, ns);
    }

    [[nodiscard]] double avgUs(std::size_t iterations) const { return (sumNs / static_cast<double>(iterations)) / 1000.0; }
    [[nodiscard]] double minUs() const { return minNs / 1000.0; }
};

struct BenchmarkResult {
    std::string engineName;
    std::size_t pieceCount;
    std::size_t twoWayCount;
    std::size_t itemCount;
    MetricStats construct;
    MetricStats nestingPreparation;
    MetricStats nesting;
    MetricStats calculationTotal;
};

BenchmarkResult runBenchmark(
    std::string_view engineName,
    nesting::AlgorithmBase& engine,
    std::span<int64_t> ids,
    std::span<std::vector<geometry::Point64>> shapes,
    std::span<std::vector<pattern::VariantConstraint>> constraints,
    std::size_t twoWayCount,
    int seamAllowance, 
    int cuttingAllowance,
    std::size_t iterations,
    std::uint32_t seed = 124125) {

    const std::size_t patternSize = ids.size();
    if (shapes.size() != patternSize || constraints.size() != patternSize || twoWayCount > patternSize) {
        throw std::invalid_argument("Mismatched span sizes or twoWayCount > patternSize");
    }

    // Prepare flipped vector
    std::vector<std::uint8_t> flipped(patternSize, 0);
    std::ranges::fill(flipped.begin(), flipped.begin() + twoWayCount, 1);

    std::mt19937 rng(seed);
    std::ranges::shuffle(flipped, rng);

    for (std::size_t i = 0; i < patternSize; ++i) {
        if (flipped[i]) {
            for (auto& c : constraints[i]) {
                c.twoWay = true;
            }
        } else {
            for (auto& c : constraints[i]) {
                c.twoWay = false;
            }
        }
    }

    // Build input vector lambda
    auto buildPatternPieces = [&]() {
        std::vector<pmg::pattern::PatternPiece> pieces;
        pieces.reserve(patternSize);
        for (std::size_t i = 0; i < patternSize; ++i) {
            pieces.emplace_back(ids[i], shapes[i], constraints[i], seamAllowance, cuttingAllowance, true);
        }
        return pieces;
    };

    // Test run
    {
        auto warmupPieces = buildPatternPieces();
        doNotOptimize(warmupPieces);
        pmg::timing::PhaseTimings dummy{};
        doNotOptimize(engine.execute(warmupPieces, &dummy));
    }

    BenchmarkResult stats{.engineName = std::string(engineName), .pieceCount = patternSize, .twoWayCount = twoWayCount};

    for (std::size_t iter = 0; iter < iterations; ++iter) {
        auto start = std::chrono::steady_clock::now();
        std::vector<pmg::pattern::PatternPiece> pieces = buildPatternPieces();
        doNotOptimize(pieces);
        auto end = std::chrono::steady_clock::now();

        stats.construct.record(std::chrono::duration<double, std::nano>(end - start).count());

        timing::PhaseTimings timings{};
        pmg::pattern::NestingResult result = engine.execute(pieces, &timings);
        doNotOptimize(result);

        stats.nestingPreparation.record(timings.preparationNs);
        stats.nesting.record(timings.calculationNs);
        stats.calculationTotal.record(timings.totalNs);

        if (iter == 0) stats.itemCount = result.positions.size();
    }

    return stats;
}

} // namespace pmg::bench

int main() {
    constexpr std::size_t ITERATIONS = 10;

    // Prepare input data
    std::vector<int64_t> ids = pmg::bench::getBenchIds();
    std::vector<std::vector<pmg::geometry::Point64>> shapes = pmg::bench::getBenchShapes();
    std::vector<std::vector<pmg::pattern::VariantConstraint>> constraints = pmg::bench::getBenchConstraints();

    std::span<int64_t> fullIds{ids};
    std::span<std::vector<pmg::geometry::Point64>> fullShapes{shapes};
    std::span<std::vector<pmg::pattern::VariantConstraint>> fullConstraints{constraints};

    constexpr int64_t width = 1500;
    constexpr int64_t height = 4500;
    const std::vector<pmg::geometry::Point64> boundary = {
        {1005, 4575}, 
        { 586, 4184}, 
        { 166, 4294}, 
        {  54,   75}, 
        {1430,  229}, 
        {1654, 2873}
    };
    pmg::pattern::Fabric regularFabric(width, height, true);
    pmg::pattern::Fabric irregularFabric(boundary, 0, true);

    // Construct algorithms and fabrics combinations
    struct AlgVariant {
        std::string name;
        std::unique_ptr<pmg::nesting::AlgorithmBase> instance;
    };

    std::vector<AlgVariant> engines;
    engines.emplace_back("QuickAlgorithm_RegularFabric", std::make_unique<pmg::nesting::BasicAlgorithm>(regularFabric));
    engines.emplace_back("QuickAlgorithm_IrregularFabric", std::make_unique<pmg::nesting::BasicAlgorithm>(irregularFabric));
    engines.emplace_back("ThoroughAlgorithm_RegularFabric", std::make_unique<pmg::nesting::AdvancedAlgorithm>(regularFabric));
    engines.emplace_back("ThoroughAlgorithm_IrregularFabric", std::make_unique<pmg::nesting::AdvancedAlgorithm>(irregularFabric));

    const std::vector<std::size_t> testSizes {2, 4, 8, 16};
    const std::vector twoWayRatios {0.0, 0.25, 0.5, 1.0};
    constexpr int seamAllowance = 15;
    constexpr int cuttingAllowance = 5;

    std::cout << "Engine,PieceSize,TwoWay,ItemSize,BuildAvg(us),BuildMin(us),PrepAvg(us),PrepMin(us),CalcAvg(us),CalcMin(us),TotalAvg(us),TotalMin(us)\n";

    for (const auto& engine : engines) {
        for (std::size_t ts : testSizes) {
            for (double ratio : twoWayRatios) {
                auto flags = static_cast<std::size_t>(static_cast<double>(ts) * ratio);
                auto r = pmg::bench::runBenchmark(
                    engine.name,
                    *engine.instance,
                    fullIds.first(ts),
                    fullShapes.first(ts),
                    fullConstraints.first(ts),
                    flags,
                    seamAllowance,
                    cuttingAllowance,
                    ITERATIONS
                );

                std::cout << std::format("{},{},{},{},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f}\n",
                    r.engineName, r.pieceCount, r.twoWayCount, r.itemCount,
                    r.construct.avgUs(ITERATIONS), r.construct.minUs(),
                    r.nestingPreparation.avgUs(ITERATIONS), r.nestingPreparation.minUs(),
                    r.nesting.avgUs(ITERATIONS), r.nesting.minUs(),
                    r.calculationTotal.avgUs(ITERATIONS), r.calculationTotal.minUs());
            }
        }
    }

    return 0;
}