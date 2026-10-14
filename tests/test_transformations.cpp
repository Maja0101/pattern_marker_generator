#include <vector>
#include <ostream>

#include <catch2/catch_amalgamated.hpp>

#include "geometry/transformations.hpp"
#include "geometry/geometry_types.hpp"
#include "point64_test_utils.hpp"

TEST_CASE("Flipping and rotating the shapes", "[trigonometry]") {
    // L shape 
    const std::vector<pmg::geometry::Point64> polygon = {
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
            pmg::geometry::transformPolygon(polygon, 0, false) == 
            std::vector<pmg::geometry::Point64>{
                {    0,     0},
                {30000,     0},
                {30000, 10000},
                {10000, 10000},
                {10000, 40000},
                {    0, 40000},
        });

        // 90 deg
        REQUIRE(
            pmg::geometry::transformPolygon(polygon, 90, false) == 
            std::vector<pmg::geometry::Point64>{
                {    0, 30000}, 
                {    0,     0}, 
                {10000,     0}, 
                {10000, 20000}, 
                {40000, 20000}, 
                {40000, 30000}
        });

        // 180 deg
        REQUIRE(
            pmg::geometry::transformPolygon(polygon, 180, false) == 
            std::vector<pmg::geometry::Point64>{
                {30000, 40000},
                {    0, 40000},
                {    0, 30000},
                {20000, 30000},
                {20000,     0},
                {30000,     0}
        });

        // 270 deg
        REQUIRE(
            pmg::geometry::transformPolygon(polygon, 270, false) == 
            std::vector<pmg::geometry::Point64>{
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
            pmg::geometry::transformPolygon(polygon, 540, false) == 
            std::vector<pmg::geometry::Point64>{
                {30000, 40000},
                {    0, 40000},
                {    0, 30000},
                {20000, 30000},
                {20000,     0},
                {30000,     0}
        });

        // -90 deg
        REQUIRE(
            pmg::geometry::transformPolygon(polygon, -90, false) == 
            std::vector<pmg::geometry::Point64>{
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
            pmg::geometry::transformPolygon(polygon, 0, true) == 
            std::vector<pmg::geometry::Point64>{
                {30000, 40000},
                {20000, 40000},
                {20000, 10000},
                {    0, 10000},
                {    0,     0},
                {30000,     0}
        });

        // 90 deg
        REQUIRE(
            pmg::geometry::transformPolygon(polygon, 90, true) == 
            std::vector<pmg::geometry::Point64>{
                {40000,     0}, 
                {40000, 10000}, 
                {10000, 10000}, 
                {10000, 30000}, 
                {    0, 30000}, 
                {    0,     0}
        });

        // 180 deg
        REQUIRE(
            pmg::geometry::transformPolygon(polygon, 180, true) == 
            std::vector<pmg::geometry::Point64>{
                {    0,     0},
                {10000,     0},
                {10000, 30000},
                {30000, 30000},
                {30000, 40000},
                {    0, 40000}
        });

        // 270 deg
        REQUIRE(
            pmg::geometry::transformPolygon(polygon, 270, true) == 
            std::vector<pmg::geometry::Point64>{
                {    0, 30000},
                {    0, 20000},
                {30000, 20000},
                {30000,     0},
                {40000,     0},
                {40000, 30000}
        });
    }

    SECTION("No flipping, custom rotation") {
        // 30 deg
        std::vector<pmg::geometry::Point64> expected = {
            {    0, 15000},
            {25981,     0},
            {30981,  8660},
            {13660, 18660},
            {28660, 44641},
            {20000, 49641}
        };
        std::vector<pmg::geometry::Point64> result = pmg::geometry::transformPolygon(polygon, 30, false);
        REQUIRE(pmg::geometry::polygonsSimilar(result, expected, 1));

        // 45 deg
        expected = {
            {    0, 21213},
            {21213,     0},
            {28284,  7071},
            {14142, 21213},
            {35355, 42426},
            {28284, 49497}
        };
        result = pmg::geometry::transformPolygon(polygon, 45, false);
        REQUIRE(pmg::geometry::polygonsSimilar(result, expected, 1));

        // 60 deg
        expected = {
            {    0, 25981},
            {15000,     0},
            {23660,  5000},
            {13660, 22321},
            {39641, 37321},
            {34641, 45981}
        };
        result = pmg::geometry::transformPolygon(polygon, 60, false);
        REQUIRE(pmg::geometry::polygonsSimilar(result, expected, 1));
    }

    SECTION("Flipping, custom rotation") {
        // 30 deg
        std::vector<pmg::geometry::Point64> expected = {
            {45981, 34641},
            {37321, 39641},
            {22321, 13660},
            { 5000, 23660},
            {    0, 15000},
            {25981,     0}
        };
        std::vector<pmg::geometry::Point64> result = pmg::geometry::transformPolygon(polygon, 30, true);
        REQUIRE(pmg::geometry::polygonsSimilar(result, expected, 1));

        // 45 deg
        expected = {
            {49497, 28284},
            {42426, 35355},
            {21213, 14142},
            { 7071, 28284},
            {    0, 21213},
            {21213,     0}
        };
        result = pmg::geometry::transformPolygon(polygon, 45, true);
        REQUIRE(pmg::geometry::polygonsSimilar(result, expected, 1));

        // 60 deg
        expected = {
            {49641, 20000},
            {44641, 28660},
            {18660, 13660},
            { 8660, 30981},
            {    0, 25981},
            {15000,     0}
        };
        result = pmg::geometry::transformPolygon(polygon, 60, true);
        REQUIRE(pmg::geometry::polygonsSimilar(result, expected, 1));
    }
}