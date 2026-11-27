#include <catch2/catch_amalgamated.hpp>

#include "utils_benchmark/benchmark_conf.hpp"
#include "pattern/shapes.hpp"
#include "nesting/basic_algorithm.hpp"
#include "nesting/advanced_algorithm.hpp"

TEST_CASE("Single case benchmark", "[benchmark]") {
    std::vector<int64_t> ids = pmg::bench::getBenchIds();
    std::vector<std::vector<pmg::geometry::Point64>> shapes = pmg::bench::getBenchShapes();
    std::vector<std::vector<pmg::pattern::VariantConstraint>> constraints = pmg::bench::getBenchConstraints();

    std::span<int64_t> fullIds{ids};
    std::span<std::vector<pmg::geometry::Point64>> fullShapes{shapes};
    std::span<std::vector<pmg::pattern::VariantConstraint>> fullConstraints{constraints};

    const std::vector<pmg::geometry::Point64> boundary = {
        {1005, 4575}, 
        { 586, 4184}, 
        { 166, 4294}, 
        {  54,   75}, 
        {1430,  229}, 
        {1654, 2873}
    };
    pmg::pattern::Fabric irregularFabric(boundary, 0, true);
    
    const std::size_t testSize = 20; // 2, 4, 8, 16, 20
    constexpr int seamAllowance = 15;
    constexpr int cuttingAllowance = 5;
    
    if (shapes.size() != ids.size() || constraints.size() != ids.size() || ids.size() < testSize) {
        throw std::invalid_argument("Mismatched sizes or size > testSize");
    }
    
    std::vector<pmg::pattern::PatternPiece> pieces;
    pieces.reserve(testSize);
    for (std::size_t i = 0; i < testSize; ++i) {
        pieces.emplace_back(ids[i], shapes[i], constraints[i], seamAllowance, cuttingAllowance, true);
    }
    
    pmg::nesting::AdvancedAlgorithm alg(irregularFabric);

    BENCHMARK("Algorithm execution time for irregular fabric and all two-way pieces") {
        return alg.execute(pieces);
    };
}