#include <cmath>

#include <catch2/catch_amalgamated.hpp>

#include "nesting/stats_recalculation.hpp"
#include "nesting/basic_algorithm.hpp"
#include "pattern/shapes.hpp"
#include "pattern/nesting_result.hpp"
#include "pattern/variant_constraint.hpp"
#include "common/geometry_types.hpp"

TEST_CASE("Recalculating statistics", "[statistics]") {
    SECTION("Recalculated statistics matching original result") {
        std::vector<pmg::geometry::Point64> fabricPts = {
            {501, 1352},
            {546, 477},
            {1540, 213},
            {1492, 1002},
            {1012, 1317}
        };
        pmg::pattern::Fabric fabric(fabricPts, 0, true);

        std::vector<pmg::pattern::PatternPiece> patternPieces;
        patternPieces.reserve(3);

        std::vector<pmg::geometry::Point64> shape1 = {
            {285, 202},
            {297, 14},
            {467, 13},
            {488, 202}
        };

        std::vector<pmg::pattern::VariantConstraint> constraints1 = {
            { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
        };

        patternPieces.emplace_back(1, shape1, constraints1, 10, 5, true);

        std::vector<pmg::geometry::Point64> shape2 = {
            {158, 497},
            {175, 337},
            {229, 309},
            {250, 288},
            {266, 261},
            {286, 266},
            {278, 329},
            {284, 417},
            {248, 423},
            {224, 443},
            {211, 466},
            {206, 497}
        };

        std::vector<pmg::pattern::VariantConstraint> constraints2 = {
            { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 0, .isMirrored = true, .requiredQuantity = 2, .twoWay = false }
        };

        patternPieces.emplace_back(2, shape2, constraints2, 10, 5, true);

        std::vector<pmg::geometry::Point64> shape3 = {
            {152, 499},
            {115, 476},
            {58, 452},
            {59, 390},
            {68, 297},
            {168, 328},
            {162, 432},
            {156, 488}
        };

        std::vector<pmg::pattern::VariantConstraint> constraints3 = {
            { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
        };

        patternPieces.emplace_back(3, shape3, constraints3, 10, 5, true);

        pmg::nesting::BasicAlgorithm alg(fabric);
        pmg::pattern::NestingResult result =  alg.execute(patternPieces);

        pmg::pattern::NestingStats newStats = pmg::nesting::checkStatsFor(fabric, patternPieces, result.positions);
        REQUIRE(newStats.usableScrapCount == result.stats.usableScrapCount);
        REQUIRE(newStats.usableScrapsPercentage == result.stats.usableScrapsPercentage);
        REQUIRE(newStats.remainingFabricPercentage == result.stats.remainingFabricPercentage);
        REQUIRE(newStats.wastePercentage == result.stats.wastePercentage);

        const int SCALE = 10000;
        REQUIRE(std::abs((newStats.usableScrapsArea/SCALE) - (result.stats.usableScrapsArea/SCALE)) <= 1);
        REQUIRE(std::abs((newStats.remainingFabricArea/SCALE) - (result.stats.remainingFabricArea/SCALE)) <= 1);
        REQUIRE(std::abs((newStats.wasteArea/SCALE) - (result.stats.wasteArea/SCALE)) <= 1);
        REQUIRE(std::abs((newStats.remainingFabricLength) - (result.stats.remainingFabricLength)) <= 1);
    }
}