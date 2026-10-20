#include <catch2/catch_amalgamated.hpp>

#include "geometry/geometry_types.hpp"
#include "geometry/decomposition_calculations.hpp"

TEST_CASE("Decomposing shapes", "[HertelMehlhorn]") {
    SECTION("Convex shape") {
        std::vector<pmg::geometry::Point64> polygon = {
                {   0,    0},  
                {4200,    0},  
                {4200, 1700}, 
                {   0, 1700}  
        };

        auto result = pmg::geometry::calculations::hertelMehlhornAlgorithm(polygon);

        std::vector<std::vector<pmg::geometry::Point64>> expected = { polygon };

        REQUIRE(result == expected);
    }

    SECTION("Simple concave shape") {
        std::vector<pmg::geometry::Point64> polygon = {
                {   0,    0},  
                {4000,    0},  
                {4000, 4000}, 
                {3000, 1000}, 
                {   0, 4000}, 
        };

        auto result = pmg::geometry::calculations::hertelMehlhornAlgorithm(polygon);

        std::vector<std::vector<pmg::geometry::Point64>> expected = { 
            {
                {   0, 4000},  
                {   0,    0},  
                {4000,    0}, 
                {3000, 1000}
            },
            {
                {4000,    0},  
                {4000, 4000},
                {3000, 1000}
            }
        };

        REQUIRE(result == expected);
    }

    SECTION("Concave shape") {
        std::vector<pmg::geometry::Point64> polygon = {
                {2000,    0},  
                {4000,    0},  
                {4000, 4000}, 
                {   0, 4000}, 
                {   0, 2000}, 
                {2000, 2000}
        };

        auto result = pmg::geometry::calculations::hertelMehlhornAlgorithm(polygon);

        std::vector<std::vector<pmg::geometry::Point64>> expected = { 
            {
                {2000,    0},  
                {4000,    0},  
                {4000, 4000}, 
                {2000, 2000}
            },
            {
                {4000, 4000},  
                {   0, 4000},  
                {   0, 2000}, 
                {2000, 2000}
            }
        };

        REQUIRE(result == expected);
    }
}