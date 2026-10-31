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
            REQUIRE(result.usableScrapsArea == 0.0);
            REQUIRE(result.usableScrapsPercentage == 0.0);
            REQUIRE(result.remainingFabricLength == 0.0);
            REQUIRE(result.remainingFabricArea == 0.0);
            REQUIRE(result.remainingFabricPercentage == 0.0);
            REQUIRE(result.wasteArea == 0.0);
            REQUIRE(result.wastePercentage == 0.0);
        }

        SECTION("No items") {
            Clipper2Lib::Paths64 placedItems;
            int64_t maxRightPoint = 0;
            
            pmg::nesting::NestingStats result = pmg::geometry::calculations::evaluate(placedItems, fabric, maxRightPoint);

            REQUIRE(result.usableScrapCount == 0);
            REQUIRE(result.usableScrapsArea == 0.0);
            REQUIRE(result.usableScrapsPercentage == 0.0);
            REQUIRE(result.remainingFabricLength == 1.0);
            REQUIRE(result.remainingFabricArea == 0.5);
            REQUIRE(result.remainingFabricPercentage == 1.0);
            REQUIRE(result.wasteArea == 0.0);
            REQUIRE(result.wastePercentage == 0.0);
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
            REQUIRE(result.usableScrapsArea == 0.0);
            REQUIRE(result.usableScrapsPercentage == 0.0);
            REQUIRE(result.remainingFabricLength == 0.6);
            REQUIRE(result.remainingFabricArea == 0.3);
            REQUIRE(result.remainingFabricPercentage == 0.6);
            REQUIRE(result.wasteArea == 0.0);
            REQUIRE(result.wastePercentage == 0.0);
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
            REQUIRE(result.usableScrapsArea == 0.04);
            REQUIRE(result.usableScrapsPercentage == 0.08);
            REQUIRE(result.remainingFabricLength == 0.4);
            REQUIRE(result.remainingFabricArea == 0.2);
            REQUIRE(result.remainingFabricPercentage == 0.4);
            REQUIRE(result.wasteArea == 0.0);
            REQUIRE(result.wastePercentage == 0.0);
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
                REQUIRE(result.usableScrapsArea == 0.2);
                REQUIRE(result.usableScrapsPercentage == 0.4);
                REQUIRE(result.remainingFabricLength == 0.0);
                REQUIRE(result.remainingFabricArea == 0.0);
                REQUIRE(result.remainingFabricPercentage == 0.0);
                REQUIRE(result.wasteArea == 0.04);
                REQUIRE(result.wastePercentage == 0.08);                
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
                REQUIRE(result.usableScrapsArea == 0.0);
                REQUIRE(result.usableScrapsPercentage == 0.0);
                REQUIRE(result.remainingFabricLength == 0.0);
                REQUIRE(result.remainingFabricArea == 0.0);
                REQUIRE(result.remainingFabricPercentage == 0.0);
                REQUIRE(result.wasteArea == 0.03);
                REQUIRE(result.wastePercentage == 0.06);    
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
        REQUIRE(result.usableScrapsArea == 0.0);
        REQUIRE(result.usableScrapsPercentage == 0.0);
        REQUIRE(result.remainingFabricLength == 0.6);
        REQUIRE(result.remainingFabricArea == 0.15);
        REQUIRE(result.remainingFabricPercentage == 0.428);
        REQUIRE(result.wasteArea == 0.0);
        REQUIRE(result.wastePercentage == 0.0);
    }
}