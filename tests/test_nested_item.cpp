#include <catch2/catch_amalgamated.hpp>

#include "nesting/nested_item.hpp"
#include "pattern/shapes.hpp"
#include "geometry/geometry_types.hpp"

TEST_CASE("NestedItem accessing path and bounding box", "[path][rect]") {
    std::vector<pmg::geometry::Point64> testBoundaryPts = {
        {15000,     0}, 
        {65000,  5000}, 
        {65000, 30000}, 
        {15000, 35000},  
        {    0, 15000}, 
        {    0, 10000}  
    };

    pmg::pattern::PieceVariant testVariant(0, false, 1, testBoundaryPts);

    pmg::nesting::NestedItem testItem(&testVariant);

    int64_t offset = 1000;

    SECTION("Boundary box calculation") {
        Clipper2Lib::Rect64 result = testItem.getItemBoundingBoxAtPosition();

        REQUIRE(result.left == 0);
        REQUIRE(result.right == 65000);
        REQUIRE(result.bottom == 35000);
        REQUIRE(result.top == 0);

        result = testItem.getItemBoundingBoxAtPosition(offset, offset);

        REQUIRE(result.left == 0 + offset);
        REQUIRE(result.right == 65000 + offset);
        REQUIRE(result.bottom == 35000 + offset);
        REQUIRE(result.top == 0 + offset);
    }

    SECTION("Path calculation") {
        Clipper2Lib::Path64 result = testItem.getItemPathAtPosition();

        Clipper2Lib::Path64 expected = {
            Clipper2Lib::Point64(15000, 0),
            Clipper2Lib::Point64(65000, 5000),
            Clipper2Lib::Point64(65000, 30000),
            Clipper2Lib::Point64(15000, 35000),
            Clipper2Lib::Point64(0, 15000),
            Clipper2Lib::Point64(0, 10000),
        };

        REQUIRE(result.size() == 6);
        REQUIRE(result == expected);

        result = testItem.getItemPathAtPosition(offset, offset);

        expected = {
            Clipper2Lib::Point64(15000 + offset, 0 + offset),
            Clipper2Lib::Point64(65000 + offset, 5000 + offset),
            Clipper2Lib::Point64(65000 + offset, 30000 + offset),
            Clipper2Lib::Point64(15000 + offset, 35000 + offset),
            Clipper2Lib::Point64(0 + offset, 15000 + offset),
            Clipper2Lib::Point64(0 + offset, 10000 + offset),
        };

        REQUIRE(result.size() == 6);
        REQUIRE(result == expected);
    }


}