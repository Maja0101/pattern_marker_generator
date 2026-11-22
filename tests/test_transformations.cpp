#include <vector>

#include <catch2/catch_amalgamated.hpp>

#include "geometry/transformations.hpp"
#include "pattern/geometry_types.hpp"
#include "point64_test_utils.hpp"

TEST_CASE("Flipping and rotating the shapes", "[trigonometry]") {
    // L shape 
    const std::vector<pmg::pattern::Point64> polygon = {
        {    0,     0},
        {30000,     0},
        {30000, 10000},
        {10000, 10000},
        {10000, 40000},
        {    0, 40000}
    };

    SECTION("No flipping, orthogonal rotation") {
        // 0 deg
        REQUIRE(
            pmg::geometry::calculations::transformPolygon(polygon, 0, false) == 
            std::vector<pmg::pattern::Point64>{
                {    0,     0},
                {30000,     0},
                {30000, 10000},
                {10000, 10000},
                {10000, 40000},
                {    0, 40000},
        });

        // 90 deg
        REQUIRE(
            pmg::geometry::calculations::transformPolygon(polygon, 90, false) == 
            std::vector<pmg::pattern::Point64>{
                {    0, 30000}, 
                {    0,     0}, 
                {10000,     0}, 
                {10000, 20000}, 
                {40000, 20000}, 
                {40000, 30000}
        });

        // 180 deg
        REQUIRE(
            pmg::geometry::calculations::transformPolygon(polygon, 180, false) == 
            std::vector<pmg::pattern::Point64>{
                {30000, 40000},
                {    0, 40000},
                {    0, 30000},
                {20000, 30000},
                {20000,     0},
                {30000,     0}
        });

        // 270 deg
        REQUIRE(
            pmg::geometry::calculations::transformPolygon(polygon, 270, false) == 
            std::vector<pmg::pattern::Point64>{
                {40000,     0},
                {40000, 30000},
                {30000, 30000},
                {30000, 10000},
                {    0, 10000},
                {    0,     0}
        });
    }

    SECTION("No flipping, orthogonal rotation, rotation angle not between 0 - 360") {
        // 540 deg
        REQUIRE(
            pmg::geometry::calculations::transformPolygon(polygon, 540, false) == 
            std::vector<pmg::pattern::Point64>{
                {30000, 40000},
                {    0, 40000},
                {    0, 30000},
                {20000, 30000},
                {20000,     0},
                {30000,     0}
        });

        // -90 deg
        REQUIRE(
            pmg::geometry::calculations::transformPolygon(polygon, -90, false) == 
            std::vector<pmg::pattern::Point64>{
                {40000,     0},
                {40000, 30000},
                {30000, 30000},
                {30000, 10000},
                {    0, 10000},
                {    0,     0}
        });
    }

    SECTION("Flipping, orthogonal rotation") {
        // 0 deg
        REQUIRE(
            pmg::geometry::calculations::transformPolygon(polygon, 0, true) == 
            std::vector<pmg::pattern::Point64>{
                {    0,     0},
                {10000,     0},
                {10000, 30000},
                {30000, 30000},
                {30000, 40000},
                {    0, 40000}
        });

        // 90 deg
        REQUIRE(
            pmg::geometry::calculations::transformPolygon(polygon, 90, true, true, false) == 
            std::vector<pmg::pattern::Point64>{
                {    0, 30000},
                {    0, 20000},
                {30000, 20000},
                {30000,     0},
                {40000,     0},
                {40000, 30000}
        });

        // 180 deg
        REQUIRE(
            pmg::geometry::calculations::transformPolygon(polygon, 180, true) == 
            std::vector<pmg::pattern::Point64>{
                {30000, 40000},
                {20000, 40000},
                {20000, 10000},
                {    0, 10000},
                {    0,     0},
                {30000,     0}
        });

        // 270 deg
        REQUIRE(
            pmg::geometry::calculations::transformPolygon(polygon, 270, true, true, false) == 
            std::vector<pmg::pattern::Point64>{
                {40000,     0},
                {40000, 10000},
                {10000, 10000},
                {10000, 30000},
                {    0, 30000},
                {    0,     0}
        });
    }

    SECTION("No flipping, custom rotation") {
        // 30 deg
        std::vector<pmg::pattern::Point64> expected = {
            {    0, 15000},
            {25981,     0},
            {30981,  8660},
            {13660, 18660},
            {28660, 44641},
            {20000, 49641}
        };
        std::vector<pmg::pattern::Point64> result = pmg::geometry::calculations::transformPolygon(polygon, 30, false);
        REQUIRE(pmg::pattern::polygonsSimilar(result, expected, 1));

        // 45 deg
        expected = {
            {    0, 21213},
            {21213,     0},
            {28284,  7071},
            {14142, 21213},
            {35355, 42426},
            {28284, 49497}
        };
        result = pmg::geometry::calculations::transformPolygon(polygon, 45, false);
        REQUIRE(pmg::pattern::polygonsSimilar(result, expected, 1));

        // 60 deg
        expected = {
            {    0, 25981},
            {15000,     0},
            {23660,  5000},
            {13660, 22321},
            {39641, 37321},
            {34641, 45981}
        };
        result = pmg::geometry::calculations::transformPolygon(polygon, 60, false);
        REQUIRE(pmg::pattern::polygonsSimilar(result, expected, 1));
    }

    SECTION("Flipping, custom rotation") {
        // 30 deg
        std::vector<pmg::pattern::Point64> expected = {
            {20000,     0}, 
            {28660,  5000}, 
            {13660, 30981}, 
            {30981, 40981}, 
            {25981, 49641}, 
            {    0, 34641}
        };
        std::vector<pmg::pattern::Point64> result = pmg::geometry::calculations::transformPolygon(polygon, 30, true);
        REQUIRE(pmg::pattern::polygonsSimilar(result, expected, 1));

        // 45 deg
        expected = {
            {28284,     0},
            {35355,  7071},
            {14142, 28284},
            {28284, 42426},
            {21213, 49497},
            {    0, 28284}
        };
        result = pmg::geometry::calculations::transformPolygon(polygon, 45, true);
        REQUIRE(pmg::pattern::polygonsSimilar(result, expected, 1));
        
        // 60 deg
        expected = {
            {34641,     0},
            {39641,  8660},
            {13660, 23660},
            {23660, 40981},
            {15000, 45981},
            {    0, 20000}
        };
        result = pmg::geometry::calculations::transformPolygon(polygon, 60, true);
        REQUIRE(pmg::pattern::polygonsSimilar(result, expected, 1));
    }
}