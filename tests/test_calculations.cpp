#include <catch2/catch_amalgamated.hpp>

#include "nesting/bl_state.hpp"
#include "geometry/calculations.hpp"
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

        REQUIRE_FALSE(pmg::geometry::calculations::utils::aabbCollision(a, b));
    }

    SECTION("Touching edge") {
        Clipper2Lib::Rect64 a(0, 0, 1000, 1000);
        Clipper2Lib::Rect64 b(1000, 0, 2000, 1000);

        REQUIRE(pmg::geometry::calculations::utils::aabbCollision(a, b));
    }

    SECTION("Collision") {
        Clipper2Lib::Rect64 a(0, 0, 1000, 1000);
        Clipper2Lib::Rect64 b(500, 500, 1500, 1500);

        REQUIRE(pmg::geometry::calculations::utils::aabbCollision(a, b));
    }
}

TEST_CASE("Checking whether a piece fits inside the fabric", "[BottomLeft][rect][difference]") {
    std::vector<pmg::geometry::Point64> fabricPts = {
        {      0,       0},
        {  20000,       0},
        {  20000,   10000},
        {  10000,   10000},
        {  10000,   20000},
        {      0,   20000}
    };
    pmg::pattern::Fabric fabric(fabricPts);
    
    pmg::nesting::BLStateView view{ 
        fabric,
        Clipper2Lib::Paths64(),
        std::vector<Clipper2Lib::Rect64>()
    };

    std::vector<pmg::geometry::Point64> variantPts = {
        {   0,    0},
        {9000,    0},
        {9000, 9000},
        {   0, 9000}
    };

    pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, 0, 0, variantPts);

    pmg::nesting::NestedItem item(&variant);

    SECTION("Bounding box outside fabric's bounding box") {
        REQUIRE_FALSE(pmg::geometry::calculations::utils::isWithinFabric(view, item, 15000, 15000));
    }

    SECTION("The piece does not fit inside the fabric") {
        REQUIRE_FALSE(pmg::geometry::calculations::utils::isWithinFabric(view, item, 5000, 5000));
    }

    SECTION("Piece is fully inside the fabric") {
        REQUIRE(pmg::geometry::calculations::utils::isWithinFabric(view, item, 500, 500));
    }

    SECTION("Piece is touching the edge of the fabric") {
        REQUIRE(pmg::geometry::calculations::utils::isWithinFabric(view, item, 0, 0));
    }
}

TEST_CASE("Checking collisions between pieces", "[BottomLeft][rect][collision]") {
    std::vector<pmg::geometry::Point64> fabricPts = {
        {    0,     0},
        {20000,     0},
        {20000, 20000},
        {    0, 20000}
    };
    pmg::pattern::Fabric fabric(fabricPts);

    Clipper2Lib::Paths64 polygons = {{
        {    0,     0},
        {10000,     0},
        {10000, 10000},
        {    0, 10000}
    }};

    Clipper2Lib::Rect64 box(0, 0, 10000, 10000);
    
    pmg::nesting::BLStateView view{ 
        fabric,
        polygons,
        std::vector<Clipper2Lib::Rect64>{box}
    };

    std::vector<pmg::geometry::Point64> variantPts = {
        {2500,    0},
        {5000,    0},
        {5000, 5000},
        {   0, 5000},
        {   0, 2500},
        {2500, 2500}
    };

    pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, 0, 0, variantPts);

    pmg::nesting::NestedItem item(&variant);

    SECTION("No potential obstacles") {
        REQUIRE_FALSE(pmg::geometry::calculations::utils::isCollision(view, item, 12500, 2500));
    }

    SECTION("Potential obstacles but no collision") {
        REQUIRE_FALSE(pmg::geometry::calculations::utils::isCollision(view, item, 8000, 8000));
    }

    SECTION("Collision with another piece") {
        REQUIRE(pmg::geometry::calculations::utils::isCollision(view, item, 7500, 2500));
    }
}

TEST_CASE("Checking if piece can be placed in position", "[BottomLeft][collision][difference]") {
    std::vector<pmg::geometry::Point64> fabricPts = {
        {    0,     0},
        {20000,     0},
        {20000, 20000},
        {    0, 20000}
    };
    pmg::pattern::Fabric fabric(fabricPts);

    Clipper2Lib::Paths64 polygons = {{
        {    0,     0},
        {10000,     0},
        {10000, 10000},
        {    0, 10000}
    }};

    Clipper2Lib::Rect64 box(0, 0, 10000, 10000);
    
    pmg::nesting::BLStateView view{ 
        fabric,
        polygons,
        std::vector<Clipper2Lib::Rect64>{box}
    };

    std::vector<pmg::geometry::Point64> variantPts = {
        {2500,    0},
        {5000,    0},
        {5000, 5000},
        {   0, 5000},
        {   0, 2500},
        {2500, 2500}
    };

    pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, 0, 0, variantPts);

    pmg::nesting::NestedItem item(&variant);

    SECTION("Valid position") {
        REQUIRE(pmg::geometry::calculations::canPlaceAt(view, item, 12500, 2500));
    }

    SECTION("Piece is not inside the fabric") {
        REQUIRE_FALSE(pmg::geometry::calculations::canPlaceAt(view, item, 16000, 16000));
    }

    SECTION("Piece cannot be placed because of collision") {
        REQUIRE_FALSE(pmg::geometry::calculations::canPlaceAt(view, item, 7500, 2500));
    }
}