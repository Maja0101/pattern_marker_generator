#include <catch2/catch_amalgamated.hpp>

#include "nesting/bl_state.hpp"
#include "nesting/calculations.hpp"
#include "geometry/geometry_types.hpp"
#include "pattern/shapes.hpp"

TEST_CASE("Detecting AABB collision", "[BottomLeft][rect][collision]") {
    std::vector<pmg::geometry::Point64> fabricPts = {
        {     0,       0},  
        {350000,      0},  
        {350000, 150000}, 
        {     0, 150000}  
    };
    pmg::pattern::Fabric fabric(fabricPts);

    SECTION("No collision") {
        Clipper2Lib::Rect64 a(0, 0, 1000, 1000); // left top right bottom
        Clipper2Lib::Rect64 b(2000, 2000, 3000, 3000);

        REQUIRE_FALSE(pmg::nesting::calculations::aabbCollision(a, b));
    }

    SECTION("Touching edge") {
        Clipper2Lib::Rect64 a(0, 0, 1000, 1000);
        Clipper2Lib::Rect64 b(1000, 0, 2000, 1000);

        REQUIRE(pmg::nesting::calculations::aabbCollision(a, b));
    }

    SECTION("Collision") {
        Clipper2Lib::Rect64 a(0, 0, 1000, 1000);
        Clipper2Lib::Rect64 b(500, 500, 1500, 1500);

        REQUIRE(pmg::nesting::calculations::aabbCollision(a, b));
    }
}