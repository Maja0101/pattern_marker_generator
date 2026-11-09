#include <vector>

#include <catch2/catch_amalgamated.hpp>

#include "geometry/transformations.hpp"
#include "geometry/geometry_types.hpp"
#include "pattern/pattern_piece.hpp"
#include "pattern/variant_constraint.hpp"
#include "nesting/basic_algorithm.hpp"
#include "nesting/nesting_result.hpp"

TEST_CASE("Input shapes transformations", "[geometry][rotations][BottomLeft]") {
    SECTION("L shape, alpha = 90 degrees, with allowances") {
        const std::vector<pmg::geometry::Point64> rectangle = {
            {1000, 1000},
            {1300, 1000},
            {1300, 1800},
            {1000, 1800}
        };

        const std::vector<pmg::geometry::Point64> polygon = {
            {400, 300},
            {500, 300},
            {600, 300},
            {600, 400},
            {600, 500},
            {500, 500},
            {500, 600},
            {500, 700},
            {400, 700}
        };

        const std::vector<pmg::geometry::Point64> expected = {
            {400000, 400000},
            {300000, 400000},
            {300000, 600000},
            {     0, 600000},
            {     0,      0},
            {400000,      0}
        };

        std::vector<pmg::pattern::VariantConstraint> constraints = {
            { .rotationAngle = 90, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        };

        pmg::pattern::PatternPiece piece1(1, polygon, constraints, 50, 50, 0, true);
        pmg::pattern::PatternPiece piece2(2, rectangle, constraints, 50, 50, 0, true);

        constraints[0].rotationAngle = 0;
        pmg::pattern::PatternPiece piece3(3, rectangle, constraints, 50, 50, 0, true);
        
        REQUIRE(piece1.getVariants()[0].getAllowancesDistance() == 100000);
        REQUIRE(piece1.getVariants()[0].getRotationAngle() == 90);
        REQUIRE(piece1.getVariants()[0].getReferencePoint().x == 200000);
        REQUIRE(piece1.getVariants()[0].getReferencePoint().y == -300000);
        REQUIRE(piece1.getVariants()[0].getBoundary(false) == expected);
        
        pmg::pattern::Fabric fabric(1600, 2000, true);
        std::vector<pmg::pattern::PatternPiece> pieces = { piece1, piece2, piece3 };
        
        pmg::nesting::BasicAlgorithm alg(fabric);

        pmg::nesting::NestingResult result = alg.execute(pieces);

        REQUIRE(result.wasSuccessful);
        REQUIRE(result.positions.size() == 3);
        REQUIRE(result.positions[0].id == 2);
        REQUIRE(result.positions[0].rotationAngle == 90);
        REQUIRE(result.positions[0].isMirrored == false);
        REQUIRE(result.positions[0].isFlipped180 == false);
        REQUIRE(result.positions[0].x == -800);
        REQUIRE(result.positions[0].y == 2800);

        REQUIRE(result.positions[1].id == 1);
        REQUIRE(result.positions[1].rotationAngle == 90);
        REQUIRE(result.positions[1].isMirrored == false);
        REQUIRE(result.positions[1].isFlipped180 == false);
        REQUIRE(result.positions[1].x == 900);
        REQUIRE(result.positions[1].y == 2200);

        REQUIRE(result.positions[2].id == 3);
        REQUIRE(result.positions[2].rotationAngle == 0);
        REQUIRE(result.positions[2].isMirrored == false);
        REQUIRE(result.positions[2].isFlipped180 == false);
        REQUIRE(result.positions[2].x == 200);
        REQUIRE(result.positions[2].y == -400);
    }
}