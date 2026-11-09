#include <catch2/catch_amalgamated.hpp>
#include <clipper2/clipper.h>

#include "pattern/shapes.hpp"
#include "geometry/calculations.hpp"
#include "nesting/nesting_result.hpp"

TEST_CASE("Calculating score for nesting result", "[evaluation]") {
    SECTION("Regular fabric") {
        std::vector<pmg::geometry::Point64> fabricPts = {
            {      0,      0},  
            {1000000,      0},  
            {1000000, 500000}, 
            {      0, 500000}  
        };
        pmg::pattern::Fabric fabric(fabricPts);

        SECTION("No fabric left") {
            Clipper2Lib::Paths64 placedItems = {
                {
                    {      0,      0},  
                    {1000000,      0},  
                    {1000000, 200000}, 
                    {      0, 200000}
                },
                {
                    {      0, 200000},  
                    { 400000, 200000},  
                    { 400000, 500000}, 
                    {      0, 500000}
                },
                {
                    { 400000, 200000},  
                    {1000000, 200000},  
                    {1000000, 500000}, 
                    { 400000, 500000}
                }
            };
            int64_t maxRightPoint = 1000000;
            
            pmg::nesting::NestingStats result = pmg::geometry::calculations::evaluate(placedItems, fabric, maxRightPoint);

            REQUIRE(result.usableScrapCount == 0);
            REQUIRE(result.usableScrapsArea == 0);
            REQUIRE(result.usableScrapsPercentage == 0);

            REQUIRE(result.effectiveScrapsArea == 0);
            REQUIRE(result.effectiveScrapsPercentage == 0);

            REQUIRE(result.remainingFabricLength == 0);
            REQUIRE(result.remainingFabricArea == 0);
            REQUIRE(result.remainingFabricPercentage == 0);

            REQUIRE(result.wasteArea == 0);
            REQUIRE(result.wastePercentage == 0);
        }

        SECTION("No items") {
            Clipper2Lib::Paths64 placedItems;
            int64_t maxRightPoint = 0;
            
            pmg::nesting::NestingStats result = pmg::geometry::calculations::evaluate(placedItems, fabric, maxRightPoint);

            REQUIRE(result.usableScrapCount == 0);
            REQUIRE(result.usableScrapsArea == 0);
            REQUIRE(result.usableScrapsPercentage == 0);

            REQUIRE(result.effectiveScrapsArea == 0);
            REQUIRE(result.effectiveScrapsPercentage == 0);

            REQUIRE(result.remainingFabricLength == 1000);
            REQUIRE(result.remainingFabricArea == 500000);
            REQUIRE(result.remainingFabricPercentage == 100);

            REQUIRE(result.wasteArea == 0);
            REQUIRE(result.wastePercentage == 0);
        }

        SECTION("Straight edge of items") {
            Clipper2Lib::Paths64 placedItems = {
                {
                    {      0,      0},  
                    { 400000,      0},  
                    { 400000, 200000}, 
                    {      0, 200000}
                },
                {
                    {      0, 200000},  
                    { 400000, 200000},  
                    { 400000, 500000}, 
                    {      0, 500000}
                }
            };
            int64_t maxRightPoint = 400000;
            
            pmg::nesting::NestingStats result = pmg::geometry::calculations::evaluate(placedItems, fabric, maxRightPoint);

            REQUIRE(result.usableScrapCount == 0);
            REQUIRE(result.usableScrapsArea == 0);
            REQUIRE(result.usableScrapsPercentage == 0);

            REQUIRE(result.effectiveScrapsArea == 0);
            REQUIRE(result.effectiveScrapsPercentage == 0);

            REQUIRE(result.remainingFabricLength == 600);
            REQUIRE(result.remainingFabricArea == 300000);
            REQUIRE(result.remainingFabricPercentage == 60);

            REQUIRE(result.wasteArea == 0);
            REQUIRE(result.wastePercentage == 0);
        }

        SECTION("Uneven edge of items") {
            Clipper2Lib::Paths64 placedItems = {
                {
                    {      0,      0},  
                    { 400000,      0},  
                    { 400000, 200000}, 
                    {      0, 200000}
                },
                {
                    {      0, 200000},  
                    { 600000, 200000},  
                    { 600000, 500000}, 
                    {      0, 500000}
                }
            };
            int64_t maxRightPoint = 600000;
            
            pmg::nesting::NestingStats result = pmg::geometry::calculations::evaluate(placedItems, fabric, maxRightPoint);

            REQUIRE(result.usableScrapCount == 1);
            REQUIRE(result.usableScrapsArea == 40000);
            REQUIRE(result.usableScrapsPercentage == 8);

            REQUIRE(result.effectiveScrapsArea == 40000);
            REQUIRE(result.effectiveScrapsPercentage == 8);

            REQUIRE(result.remainingFabricLength == 400);
            REQUIRE(result.remainingFabricArea == 200000);
            REQUIRE(result.remainingFabricPercentage == 40);

            REQUIRE(result.wasteArea == 0);
            REQUIRE(result.wastePercentage == 0);
        }

        SECTION("Scraps smaller than minimum") {
            SECTION("Too narrow scrap") {
                Clipper2Lib::Paths64 placedItems = {
                    {
                        {      0,      0},  
                        { 100000,      0},  
                        { 100000, 500000}, 
                        {      0, 500000}
                    },
                    {
                        { 900000,      0},  
                        {1000000,      0},  
                        {1000000, 500000}, 
                        { 900000, 500000}
                    },
                    {
                        { 300000,      0},  
                        { 700000,      0},  
                        { 700000, 200000}, 
                        { 300000, 200000}
                    },
                    {
                        { 300000, 300000},  
                        { 700000, 300000},  
                        { 700000, 500000}, 
                        { 300000, 500000}
                    }
                };
                int64_t maxRightPoint = 1000000;
                
                pmg::nesting::NestingStats result = pmg::geometry::calculations::evaluate(placedItems, fabric, maxRightPoint);

                REQUIRE(result.usableScrapCount == 2);
                REQUIRE(result.usableScrapsArea == 200000);
                REQUIRE(result.usableScrapsPercentage == 40);

                REQUIRE(result.effectiveScrapsArea == 200000);
                REQUIRE(result.effectiveScrapsPercentage == 40);

                REQUIRE(result.remainingFabricLength == 0);
                REQUIRE(result.remainingFabricArea == 0);
                REQUIRE(result.remainingFabricPercentage == 0);

                REQUIRE(result.wasteArea == 40000);
                REQUIRE(result.wastePercentage == 8);                
            }

            SECTION("Scrap with too small area") {
                Clipper2Lib::Paths64 placedItems = {
                    {
                        {      0,      0},  
                        { 300000,      0},  
                        { 300000, 500000}, 
                        {      0, 500000}
                    },
                    {
                        { 300000,      0},  
                        {1000000,      0},  
                        {1000000, 500000}, 
                        { 450000, 500000},
                        { 450000, 300000},
                        { 300000, 300000}
                    }
                };
                int64_t maxRightPoint = 1000000;
                
                pmg::nesting::NestingStats result = pmg::geometry::calculations::evaluate(placedItems, fabric, maxRightPoint);

                REQUIRE(result.usableScrapCount == 0);
                REQUIRE(result.usableScrapsArea == 0);
                REQUIRE(result.usableScrapsPercentage == 0);

                REQUIRE(result.effectiveScrapsArea == 0);
                REQUIRE(result.effectiveScrapsPercentage == 0);

                REQUIRE(result.remainingFabricLength == 0);
                REQUIRE(result.remainingFabricArea == 0);
                REQUIRE(result.remainingFabricPercentage == 0);

                REQUIRE(result.wasteArea == 30000);
                REQUIRE(result.wastePercentage == 6);    
            }

            SECTION("Irregular scrap with smaller effective area") {
                Clipper2Lib::Paths64 placedItems = {
                    {
                        {      0,      0},  
                        { 200000,      0},  
                        { 200000, 200000}, 
                        {      0, 200000}
                    },
                    {
                        { 400000,      0},  
                        { 600000,      0},  
                        { 600000, 500000}, 
                        {      0, 500000},
                        {      0, 400000},
                        { 400000, 400000}
                    }
                };
                int64_t maxRightPoint = 600000;
                
                pmg::nesting::NestingStats result = pmg::geometry::calculations::evaluate(placedItems, fabric, maxRightPoint);

                REQUIRE(result.usableScrapCount == 1);
                REQUIRE(result.usableScrapsArea == 120000);
                REQUIRE(result.usableScrapsPercentage == 24);

                REQUIRE(result.effectiveScrapsArea == 102857);
                REQUIRE(result.effectiveScrapsPercentage == 21);

                REQUIRE(result.remainingFabricLength == 400);
                REQUIRE(result.remainingFabricArea == 200000);
                REQUIRE(result.remainingFabricPercentage == 40);

                REQUIRE(result.wasteArea == 0);
                REQUIRE(result.wastePercentage == 0);    
            }
        }
    }

    SECTION("Irregular fabric") {
        std::vector<pmg::geometry::Point64> fabricPts = {
            {      0,      0},  
            { 400000,      0},  
            {1000000, 300000}, 
            { 400000, 500000}, 
            {      0, 500000}  
        };
        pmg::pattern::Fabric fabric(fabricPts);

        Clipper2Lib::Paths64 placedItems = {
            {
                {      0,      0},  
                { 400000,      0},  
                { 400000, 500000}, 
                {      0, 500000}
            }
        };
        int64_t maxRightPoint = 400000;
        
        pmg::nesting::NestingStats result = pmg::geometry::calculations::evaluate(placedItems, fabric, maxRightPoint);

        REQUIRE(result.usableScrapCount == 0);
        REQUIRE(result.usableScrapsArea == 0);
        REQUIRE(result.usableScrapsPercentage == 0);

        REQUIRE(result.effectiveScrapsArea == 0);
        REQUIRE(result.effectiveScrapsPercentage == 0);

        REQUIRE(result.remainingFabricLength == 600);
        REQUIRE(result.remainingFabricArea == 150000);
        REQUIRE(result.remainingFabricPercentage == 43);

        REQUIRE(result.wasteArea == 0);
        REQUIRE(result.wastePercentage == 0);
    }
}