#include <catch2/catch_amalgamated.hpp>

#include "geometry/geometry_types.hpp"
#include "pattern/shapes.hpp"
#include "nesting/nested_item.hpp"
#include "geometry/options_calculations.hpp"

TEST_CASE("Calculating number of options", "[possibilities][evaluation]") {
    std::vector<pmg::geometry::Point64> boundary1 = {
        {    0,     0},
        {10000,     0},
        {10000, 10000}
    };

    std::vector<pmg::geometry::Point64> boundary2 = {
        {    0,     0},
        {20000,     0},
        {20000, 20000}
    };

    std::vector<pmg::geometry::Point64> boundary3 = {
        {    0,     0},
        {30000,     0},
        {30000, 30000}
    };

    SECTION("Total above limit") {
        pmg::pattern::PieceVariant variant1(1, 1, 0, false, 1, false, 0, 0, boundary1);
        pmg::pattern::PieceVariant variant2(2, 1, 0, false, 1, false, 0, 0, boundary2);
        pmg::pattern::PieceVariant variant3(3, 1, 0, false, 1, false, 0, 0, boundary3);

        std::vector<pmg::nesting::NestedItem> items = { 
            pmg::nesting::NestedItem(&variant1),
            pmg::nesting::NestedItem(&variant2),
            pmg::nesting::NestedItem(&variant3)
        };

        REQUIRE_FALSE(pmg::geometry::calculations::numberOfOptionsWithinLimit(items, 5));
    }

    SECTION("Too many two way pieces") {
        pmg::pattern::PieceVariant variant1(1, 1, 0, false, 1, true, 0, 0, boundary1);
        pmg::pattern::PieceVariant variant2(2, 1, 0, false, 1, true, 0, 0, boundary2);
        pmg::pattern::PieceVariant variant3(3, 1, 0, false, 1, true, 0, 0, boundary3);

        std::vector<pmg::nesting::NestedItem> items = { 
            pmg::nesting::NestedItem(&variant1),
            pmg::nesting::NestedItem(&variant2),
            pmg::nesting::NestedItem(&variant3)
        };

        REQUIRE_FALSE(pmg::geometry::calculations::numberOfOptionsWithinLimit(items, 7));
    }

    SECTION("Total above limit with two way pieces") {
        pmg::pattern::PieceVariant variant1(1, 1, 0, false, 1, true, 0, 0, boundary1);
        pmg::pattern::PieceVariant variant2(2, 1, 0, false, 1, true, 0, 0, boundary2);
        pmg::pattern::PieceVariant variant3(3, 1, 0, false, 1, true, 0, 0, boundary3);

        std::vector<pmg::nesting::NestedItem> items = { 
            pmg::nesting::NestedItem(&variant1),
            pmg::nesting::NestedItem(&variant2),
            pmg::nesting::NestedItem(&variant3)
        };

        REQUIRE_FALSE(pmg::geometry::calculations::numberOfOptionsWithinLimit(items, 10));
    }

    SECTION("Total within limit") {
        pmg::pattern::PieceVariant variant1(1, 1, 0, false, 1, true, 0, 0, boundary1);
        pmg::pattern::PieceVariant variant2(2, 1, 0, false, 1, true, 0, 0, boundary2);
        pmg::pattern::PieceVariant variant3(3, 1, 0, false, 1, true, 0, 0, boundary3);

        std::vector<pmg::nesting::NestedItem> items = { 
            pmg::nesting::NestedItem(&variant1),
            pmg::nesting::NestedItem(&variant2),
            pmg::nesting::NestedItem(&variant3)
        };

        REQUIRE(pmg::geometry::calculations::numberOfOptionsWithinLimit(items, 50));
    }

    SECTION("Do not count the same order as two options") {
        pmg::pattern::PieceVariant variant1(1, 1, 0, false, 2, false, 0, 0, boundary1);
        pmg::pattern::PieceVariant variant2(2, 1, 0, false, 2, false, 0, 0, boundary2);
        pmg::pattern::PieceVariant variant3(3, 1, 0, false, 2, false, 0, 0, boundary3);

        std::vector<pmg::nesting::NestedItem> items = { 
            pmg::nesting::NestedItem(&variant1),
            pmg::nesting::NestedItem(&variant2),
            pmg::nesting::NestedItem(&variant3)
        };

        REQUIRE(pmg::geometry::calculations::numberOfOptionsWithinLimit(items, 6));
    }
}