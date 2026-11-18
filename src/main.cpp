#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <vector>
#include <memory>
#include <iostream>

#include <spdlog/spdlog.h>
#include <clipper2/clipper.h>

#include "utils/logger_setup.hpp"
#include "utils/timer.hpp"
#include "utils/config.hpp"
#include "geometry/geometry_types.hpp"
#include "geometry/transformations.hpp"
#include "pattern/pattern_piece.hpp"
#include "pattern/shapes.hpp"
#include "nesting/nested_item.hpp"
#include "nesting/basic_algorithm.hpp"
#include "nesting/advanced_algorithm.hpp"
#include "nesting/nesting_result.hpp"
#include "sample_input.hpp"

using namespace pmg;

int main()
{
    logging::initialize();

    if constexpr (config::MAIN_DEBUG) {

        if constexpr (config::DEVELOPMENT) SPDLOG_INFO("Starting Pattern Marker Generator...");

        std::vector<geometry::Point64> fabricPts = {
            {501, 1352},
            {546, 477},
            {1540, 213},
            {1492, 1002},
            {1012, 1317}
        };

        // std::vector<geometry::Point64> fabricPts = {
        //     {0, 1686},
        //     {59, 415},
        //     {1531, 0},
        //     {1424, 1211},
        //     {760, 1638}
        // };
        
        // Verify the result and compatibilty of the algorithm
        // pattern::Fabric fabric(1000, 750, true);
        pattern::Fabric fabric(fabricPts, 0, true);
        nesting::BasicAlgorithm alg(fabric);
        std::vector<pmg::pattern::PatternPiece> piecesToCheck = samples::getPiecesToCheck();
        nesting::NestingResult result =  alg.execute(piecesToCheck);

        // Verification of results for new version of advanced algorithm with complex pattern
        // pattern::Fabric fabric(1400, 4500, true);
        // nesting::AdvancedAlgorithm alg(fabric);
        // std::vector<pmg::pattern::PatternPiece> testPieces = samples::getTestPieces();
        // nesting::NestingResult result =  alg.execute(testPieces);


        if constexpr (config::DEVELOPMENT) SPDLOG_INFO(std::format("The nesting was{} finished", (result.wasSuccessful ? "" : "n't")));
        if constexpr (config::DEVELOPMENT) SPDLOG_INFO(std::format("The minmal length of fabric needed is {} mm", result.minLength));
        if constexpr (config::DEVELOPMENT) SPDLOG_INFO(std::format("Execution time: {} ms", result.executionTime));

        if constexpr (config::DEVELOPMENT) SPDLOG_INFO(std::format("Scraps: {}, {} smm {}% Fabric: {} mm, {} smm {}% Waste: {} smm {}%", 
            result.stats.usableScrapCount, result.stats.usableScrapsArea, result.stats.usableScrapsPercentage,
            result.stats.remainingFabricLength, result.stats.remainingFabricArea, result.stats.remainingFabricPercentage,
            result.stats.wasteArea, result.stats.wastePercentage)
        );

    }

    return 0;
}


