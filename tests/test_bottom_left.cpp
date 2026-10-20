#include <catch2/catch_amalgamated.hpp>

#include "nesting/bottom_left.hpp"
#include "pattern/shapes.hpp"
#include "geometry/nfp_ifp_calculations.hpp"
#include "geometry/nfp_key.hpp"

TEST_CASE("Nesting pieces with Bottom Left", "[BottomLeft]") {
    struct ExpectedItem {
        int64_t x;
        int64_t y;
        bool locked;
    };

    auto checkNestedItem = [](const pmg::nesting::NestedItem& item, ExpectedItem expected) {
        CHECK(item.getPosition().x == expected.x);
        CHECK(item.getPosition().y == expected.y);
        CHECK(item.isLocked() == expected.locked);
    };

    SECTION("Regular fabric") {
        std::vector<pmg::geometry::Point64> fabricPts = {
            {    0,     0},
            {20000,     0},
            {20000, 20000},
            {    0, 20000}
        };
        pmg::pattern::Fabric fabric(fabricPts);
        
        SECTION("No pieces to place") {
            std::unordered_map<int64_t, Clipper2Lib::Paths64> ifpEmptyCache;
            std::unordered_map<pmg::geometry::NFPKey, Clipper2Lib::Paths64> nfpEmptyCache;
            pmg::nesting::BottomLeft bottomLeft(fabric, ifpEmptyCache, nfpEmptyCache);

            std::vector<pmg::nesting::NestedItem> items;

            REQUIRE(bottomLeft.performNesting(items));
            REQUIRE(items.empty());
        }

        SECTION("One piece to place") {
            std::vector<pmg::geometry::Point64> variantPts = {
                {2500,    0},
                {5000,    0},
                {5000, 5000},
                {   0, 5000},
                {   0, 2500},
                {2500, 2500}
            };

            pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, variantPts);

            std::vector<pmg::nesting::NestedItem> items = { pmg::nesting::NestedItem(&variant) };

            std::unordered_map<int64_t, Clipper2Lib::Paths64> ifpCache = pmg::geometry::calculations::calculateIFPCache(fabric, { variant });;
            std::unordered_map<pmg::geometry::NFPKey, Clipper2Lib::Paths64> nfpCache = pmg::geometry::calculations::calculateNFPCache({ variant });;
            pmg::nesting::BottomLeft bottomLeft(fabric, ifpCache, nfpCache);

            REQUIRE(items.size() == 1);
            checkNestedItem(items[0], {0, 0, false});

            REQUIRE(bottomLeft.performNesting(items));
            REQUIRE(items.size() == 1);
            checkNestedItem(items[0], {0, 0, true});
        }

        SECTION("Multiple pieces to place") {
            SECTION("Rectangle shapes") {
                std::vector<pmg::geometry::Point64> rectanglePts = {
                    {   0,    0},  
                    {4000,    0},  
                    {4000, 2000}, 
                    {   0, 2000}  
                };
                std::vector<pmg::geometry::Point64> tallRectPts = {
                    {   0,     0},  
                    {2000,     0},  
                    {2000, 18000}, 
                    {   0, 18000}  
                };
                std::vector<pmg::geometry::Point64> smallRectPts = {
                    {   0,    0},  
                    {2000,    0},  
                    {2000, 1000}, 
                    {   0, 1000}  
                };

                pmg::pattern::PieceVariant variant1(1, 1, 0, false, 1, rectanglePts);
                pmg::pattern::PieceVariant variant2(2, 2, 0, false, 1, tallRectPts);
                pmg::pattern::PieceVariant variant3(3, 3, 0, false, 1, smallRectPts);

                std::vector<pmg::nesting::NestedItem> items = { 
                    pmg::nesting::NestedItem(&variant1), 
                    pmg::nesting::NestedItem(&variant2),
                    pmg::nesting::NestedItem(&variant3)
                };

                std::unordered_map<int64_t, Clipper2Lib::Paths64> ifpCache = pmg::geometry::calculations::calculateIFPCache(fabric, { variant1, variant2, variant3 });;
                std::unordered_map<pmg::geometry::NFPKey, Clipper2Lib::Paths64> nfpCache = pmg::geometry::calculations::calculateNFPCache({ variant1, variant2, variant3 });;
                pmg::nesting::BottomLeft bottomLeft(fabric, ifpCache, nfpCache);

                REQUIRE(items.size() == 3);
                for (size_t i = 0; i < items.size(); ++i) {
                    CAPTURE(i);
                    checkNestedItem(items[i], {0, 0, false});
                }

                REQUIRE(bottomLeft.performNesting(items));
                
                const std::vector<ExpectedItem> expected = {
                    {0, 0, true},
                    {0, 2000, true},
                    {2000, 2000, true}
                };
                
                REQUIRE(items.size() == 3);
                for (size_t i = 0; i < items.size(); ++i) {
                    CAPTURE(i);
                    checkNestedItem(items[i], expected[i]);
                }
            }

            SECTION("Irregular shapes") {
                std::vector<pmg::geometry::Point64> firstTrainglePts = {
                    {    0,    0},  
                    {10000,    0},  
                    {    0, 5000}
                };
                std::vector<pmg::geometry::Point64> secondTrainglePts = {
                    {10000,    0},  
                    {10000, 5000},  
                    {    0, 5000}
                };

                pmg::pattern::PieceVariant variant1(1, 1, 0, false, 1, firstTrainglePts);
                pmg::pattern::PieceVariant variant2(2, 2, 0, false, 1, secondTrainglePts);

                std::vector<pmg::nesting::NestedItem> items = { 
                    pmg::nesting::NestedItem(&variant1), 
                    pmg::nesting::NestedItem(&variant2)
                };

                std::unordered_map<int64_t, Clipper2Lib::Paths64> ifpCache = pmg::geometry::calculations::calculateIFPCache(fabric, { variant1, variant2 });;
                std::unordered_map<pmg::geometry::NFPKey, Clipper2Lib::Paths64> nfpCache = pmg::geometry::calculations::calculateNFPCache({ variant1, variant2 });;
                pmg::nesting::BottomLeft bottomLeft(fabric, ifpCache, nfpCache);

                REQUIRE(items.size() == 2);
                for (size_t i = 0; i < items.size(); ++i) {
                    CAPTURE(i);
                    checkNestedItem(items[i], {0, 0, false});
                }

                REQUIRE(bottomLeft.performNesting(items));
                
                REQUIRE(items.size() == 2);
                for (size_t i = 0; i < items.size(); ++i) {
                    CAPTURE(i);
                    checkNestedItem(items[i], {0, 0, true});
                }
            }

            SECTION("Hole filling") {
                std::vector<pmg::geometry::Point64> invVShape = {
                    {    0,    0},  
                    { 4000,    0},  
                    { 4000, 6000},
                    {    0, 6000},
                    {    0, 4000},
                    { 2000, 4000},
                    { 2000, 2000},
                    {    0, 2000}
                };
                std::vector<pmg::geometry::Point64> rectShape = {
                    {   0,    0},  
                    {2000,    0},  
                    {2000, 1000}, 
                    {   0, 1000}  
                };

                pmg::pattern::PieceVariant variant1(1, 1, 0, false, 1, invVShape);
                pmg::pattern::PieceVariant variant2(2, 2, 0, false, 1, rectShape);

                std::vector<pmg::nesting::NestedItem> items = { 
                    pmg::nesting::NestedItem(&variant1), 
                    pmg::nesting::NestedItem(&variant2)
                };

                std::unordered_map<int64_t, Clipper2Lib::Paths64> ifpCache = pmg::geometry::calculations::calculateIFPCache(fabric, { variant1, variant2 });;
                std::unordered_map<pmg::geometry::NFPKey, Clipper2Lib::Paths64> nfpCache = pmg::geometry::calculations::calculateNFPCache({ variant1, variant2 });;
                pmg::nesting::BottomLeft bottomLeft(fabric, ifpCache, nfpCache);

                REQUIRE(items.size() == 2);
                for (size_t i = 0; i < items.size(); ++i) {
                    CAPTURE(i);
                    checkNestedItem(items[i], {0, 0, false});
                }

                REQUIRE(bottomLeft.performNesting(items));
                
                const std::vector<ExpectedItem> expected = {
                    {0, 0, true},
                    {0, 2000, true}
                };
                
                REQUIRE(items.size() == 2);
                for (size_t i = 0; i < items.size(); ++i) {
                    CAPTURE(i);
                    checkNestedItem(items[i], expected[i]);
                }
            }
        }

        SECTION("Impossible to find placement") {
            std::vector<pmg::geometry::Point64> squarePts = {
                {   0,      0},  
                { 9000,     0},  
                { 9000,  9000}, 
                {    0,  9000}  
            };
            std::vector<pmg::geometry::Point64> rectanglePts = {
                {   0,      0},  
                {15000,     0},  
                {15000,  9000}, 
                {    0,  9000}  
            };

            pmg::pattern::PieceVariant variantSquare(1, 1, 0, false, 1, squarePts);
            pmg::pattern::PieceVariant variantRect(2, 2, 0, false, 1, rectanglePts);

            std::vector<pmg::nesting::NestedItem> items = { 
                pmg::nesting::NestedItem(&variantSquare), 
                pmg::nesting::NestedItem(&variantSquare), 
                pmg::nesting::NestedItem(&variantSquare), 
                pmg::nesting::NestedItem(&variantRect)
            };

            std::unordered_map<int64_t, Clipper2Lib::Paths64> ifpCache = pmg::geometry::calculations::calculateIFPCache(fabric, { variantSquare, variantRect });;
            std::unordered_map<pmg::geometry::NFPKey, Clipper2Lib::Paths64> nfpCache = pmg::geometry::calculations::calculateNFPCache({ variantSquare, variantRect });;
            pmg::nesting::BottomLeft bottomLeft(fabric, ifpCache, nfpCache);

            for (size_t i = 0; i < items.size(); ++i) {
                CAPTURE(i);
                checkNestedItem(items[i], {0, 0, false});
            }

            REQUIRE(items.size() == 4);
            REQUIRE_FALSE(bottomLeft.performNesting(items));

            const std::vector<ExpectedItem> expected = {
                {0, 0, true},
                {0, 9000, true},
                {9000, 0, true},
                {0, 0, false}
            };

            REQUIRE(items.size() == 4);
            for (size_t i = 0; i < items.size(); ++i) {
                CAPTURE(i);
                checkNestedItem(items[i], expected[i]);
            }
        }
    }
}