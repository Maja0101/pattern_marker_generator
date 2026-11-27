#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <vector>
#include <memory>
#include <iostream>

#include <spdlog/spdlog.h>
#include <clipper2/clipper.h>

#include "utils/logger_setup.hpp"
#include "utils/timer.hpp"
#include "utils/config.hpp"
#include "common/geometry_types.hpp"
#include "geometry_preparation/transformations.hpp"
#include "pattern/pattern_piece.hpp"
#include "pattern/shapes.hpp"
#include "pattern/nested_item.hpp"
#include "nesting/basic_algorithm.hpp"
#include "nesting/advanced_algorithm.hpp"
#include "nesting/stats_recalculation.hpp"
#include "pattern/nesting_result.hpp"
#include "sample_input.hpp"

using namespace pmg;

int main()
{
    logging::initialize();

    if constexpr (config::MAIN_DEBUG) {

        if constexpr (config::DEVELOPMENT) SPDLOG_INFO("Starting Pattern Marker Generator...");

        // std::vector<geometry::Point64> fabricPts = {
        //     {501, 1352},
        //     {546, 477},
        //     {1540, 213},
        //     {1492, 1002},
        //     {1012, 1317}
        // };

        // std::vector<geometry::Point64> fabricPts = {
        //     {0, 1686},
        //     {59, 415},
        //     {1531, 0},
        //     {1424, 1211},
        //     {760, 1638}
        // };

        // std::vector<geometry::Point64> fabricPts = {
        //     {593, 1582},
        //     {640, 574},
        //     {1808, 245},
        //     {1723, 1205},
        //     {1196, 1545}
        // };

        std::vector<geometry::Point64> fabricPts3 = {
            {93850, 1761690},
            {138394, 950886},
            {621773, 44442},
            {954470, 52019},
            {1210982, 235110},
            {1222810, 1914470},
            {691661, 1908122}
        };

        std::vector<geometry::Point64> fabricPts4 = {
            {1608496, 1935107},
            {1087658, 1982983},
            {510578, 1538183},
            {48914, 1208777},
            {831545, 74270},
            {1349451, 265775},
            {1766231, 1417060}
        };
        
        // Verify the result and compatibilty of the algorithm
        // pattern::Fabric fabric(1000, 750, true);
        // pattern::Fabric fabric(fabricPts3, 145, false);
        // nesting::BasicAlgorithm alg(fabric);
        // std::vector<pmg::pattern::PatternPiece> piecesToCheck = samples::getPiecesToCheck3();
        // pattern::NestingResult result = alg.execute(piecesToCheck);

        // Verification of results for new version of advanced algorithm with complex pattern
        pattern::Fabric fabric(1400, 4500, true);
        nesting::BasicAlgorithm alg(fabric);
        std::vector<pmg::pattern::PatternPiece> testPieces = samples::getTestPieces();
        pattern::NestingResult result = alg.execute(testPieces);


        if constexpr (config::DEVELOPMENT) SPDLOG_INFO(std::format("The nesting was{} finished", (result.wasSuccessful ? "" : "n't")));
        if constexpr (config::DEVELOPMENT) SPDLOG_INFO(std::format("The minmal length of fabric needed is {} mm", result.minLength));
        if constexpr (config::DEVELOPMENT) SPDLOG_INFO(std::format("Execution time: {} ms", result.executionTime));

        if constexpr (config::DEVELOPMENT) SPDLOG_INFO(std::format("Scraps: {}, {} smm {}% Fabric: {} mm, {} smm {}% Waste: {} smm {}%", 
            result.stats.usableScrapCount, result.stats.usableScrapsArea, result.stats.usableScrapsPercentage,
            result.stats.remainingFabricLength, result.stats.remainingFabricArea, result.stats.remainingFabricPercentage,
            result.stats.wasteArea, result.stats.wastePercentage)
        );
        
        if constexpr (config::DEBUG_MODE_DETAILED) {
            for (const auto& p : result.positions) {
                SPDLOG_INFO(std::format("{} {} {} {} ({}, {})", p.id, p.rotationAngle, p.isMirrored, p.isFlipped180, p.x, p.y));
            }
        }

        // if constexpr (config::DEBUG_MODE_DETAILED) {
        //     pattern::NestingStats newStats = nesting::checkStatsFor(fabric, testPieces, result.positions);
        //     SPDLOG_INFO(std::format("Scraps: {}, {} smm {}% Fabric: {} mm, {} smm {}% Waste: {} smm {}%", 
        //         newStats.usableScrapCount, newStats.usableScrapsArea, newStats.usableScrapsPercentage,
        //         newStats.remainingFabricLength, newStats.remainingFabricArea, newStats.remainingFabricPercentage,
        //         newStats.wasteArea, newStats.wastePercentage)
        //     );
        // } 

    }

    return 0;
}


