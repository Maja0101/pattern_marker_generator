#include <catch2/catch_amalgamated.hpp>
#include <clipper2/clipper.h>
#include <spdlog/spdlog.h>

#include "geometry/geometry_types.hpp"
#include "geometry/nfp_ifp_calculations.hpp"
#include "pattern/shapes.hpp"

TEST_CASE("Calculating IFP", "[IFP]") {
    SECTION("Rectangular fabric") {
        std::vector<pmg::geometry::Point64> fabricPts = {
            {   0,    0},  
            {6000,    0},  
            {6000, 6000}, 
            {   0, 6000} 
        };
        pmg::pattern::Fabric fabric(fabricPts);

        std::vector<pmg::geometry::Point64> polygon = {
            {2000,    0},  
            {4000,    0},  
            {4000, 4000}, 
            {   0, 4000}, 
            {   0, 2000}, 
            {2000, 2000}
        };
        pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, false, 0, 0, polygon);

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildIFPfor(fabric, variant);

        Clipper2Lib::Paths64 expected = {{
            {   0,    0},  
            {2000,    0},  
            {2000, 2000}, 
            {   0, 2000} 
        }};

        REQUIRE(result == expected);
    }

    SECTION("Piece does not fit inside fabric") {
        std::vector<pmg::geometry::Point64> fabricPts = {
            {   0,    0},  
            {6000,    0},  
            {6000, 6000}, 
            {   0, 6000} 
        };
        pmg::pattern::Fabric fabric(fabricPts);

        std::vector<pmg::geometry::Point64> polygon = {
            {   0,    0},  
            {7000,    0},  
            {7000, 6000}, 
            {   0, 6000} 
        };
        pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, false, 0, 0, polygon);

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildIFPfor(fabric, variant);

        Clipper2Lib::Paths64 expected;

        REQUIRE(result == expected);
    }

    SECTION("Irregular convex fabric shape") {
        std::vector<pmg::geometry::Point64> fabricPts = {
            {   0,     0},  
            {9000,     0},  
            {6000,  6000}, 
            {   0,  6000}
        };
        pmg::pattern::Fabric fabric(fabricPts);

        std::vector<pmg::geometry::Point64> polygon = {
            {   0,    0},  
            {2000,    0},  
            {2000, 2000}, 
            {   0, 2000} 
        };
        pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, false, 0, 0, polygon);

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildIFPfor(fabric, variant, 10000);

        Clipper2Lib::Paths64 expected = {{
            {6000,    0},
            {4000, 4000},  
            {   0, 4000}, 
            {   0,    0} 
        }};

        REQUIRE(result == expected);
    }

    SECTION("Irregular convex fabric shape and piece") {
        std::vector<pmg::geometry::Point64> fabricPts = {
            {3000,     0},  
            {6000,     0},  
            {6000,  6000}, 
            {   0,  6000},
            {   0,  3000}
        };
        pmg::pattern::Fabric fabric(fabricPts);

        std::vector<pmg::geometry::Point64> polygon = {
            {2000,     0},  
            {4000,     0},  
            {4000,  4000}, 
            {   0,  4000},
            {   0,  2000}
        };
        pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, false, 0, 0, polygon);

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildIFPfor(fabric, variant);

        Clipper2Lib::Paths64 expected = {{
            {1000,    0}, 
            {2000,    0}, 
            {2000, 2000},  
            {   0, 2000},  
            {   0, 1000}
        }};

        REQUIRE(result == expected);
    }

    SECTION("Irregular concave fabric shape (one pocket) and piece") {
        std::vector<pmg::geometry::Point64> fabricPts = {
            {3000,     0},  
            {6000,     0},  
            {6000,  6000}, 
            {   0,  6000},
            {   0,  3000},
            {3000,  3000}
        };
        pmg::pattern::Fabric fabric(fabricPts);

        std::vector<pmg::geometry::Point64> polygon = {
            {2000,     0},  
            {4000,     0},  
            {4000,  4000}, 
            {   0,  4000},
            {   0,  2000},
            {2000,  2000}
        };
        pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, false, 0, 0, polygon);

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildIFPfor(fabric, variant);

        Clipper2Lib::Paths64 expected = {{
            {2000, 2000}, 
            {1000, 2000},  
            {1000, 1000},  
            {2000, 1000}
        }};

        REQUIRE(result == expected);
    }

    SECTION("Irregular concave fabric shape (multiple pockets) and piece") {
        std::vector<pmg::geometry::Point64> fabricPts = {
            {3000,     0},  
            {6000,     0},  
            {6000,  5000}, 
            {7000,  5000}, 
            {7000,  6000}, 
            {5000,  6000}, 
            {5000,  5000}, 
            {3000,  5000}, 
            {3000,  6000}, 
            {   0,  6000},
            {   0,  3000},
            {3000,  3000}
        };
        pmg::pattern::Fabric fabric(fabricPts);

        std::vector<pmg::geometry::Point64> polygon = {
            {1000,     0},  
            {2000,     0},  
            {2000,  2000}, 
            {   0,  2000},
            {   0,  1000},
            {1000,  1000}
        };
        pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, false, 0, 0, polygon);

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildIFPfor(fabric, variant);

        Clipper2Lib::Paths64 expected = {
            {
                {1000, 3000}, 
                {1000, 4000},  
                {   0, 4000},  
                {   0, 3000}
            },
            {
                {2000, 2000}, 
                {3000, 2000},  
                {3000,    0},  
                {4000,    0},  
                {4000, 3000},
                {2000, 3000}
            },
        };

        REQUIRE(result == expected);
    }
}

TEST_CASE("Calculating NFP", "[NFP]") {
    SECTION("Convex shapes") {
        std::vector<pmg::geometry::Point64> polygon1 = {
            {   0,    0},  
            {4000,    0},  
            {4000, 4000}, 
            {   0, 4000} 
        };
        pmg::pattern::PieceVariant variant1(1, 1, 0, false, 1, false, 0, 0, polygon1);

        std::vector<pmg::geometry::Point64> polygon2 = {
            {   0,    0},  
            {2000,    0},  
            {2000, 2000}, 
            {   0, 2000} 
        };
        pmg::pattern::PieceVariant variant2(2, 2, 0, false, 1, false, 0, 0, polygon2);

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildNFPfor(variant1, variant2);

        Clipper2Lib::Paths64 expected = {{
            { 4000, -2000},  
            { 4000,  4000},  
            {-2000,  4000}, 
            {-2000, -2000} 
        }};

        REQUIRE(result == expected);
    }

    SECTION("Concave shapes ") {
        std::vector<pmg::geometry::Point64> polygon1 = {
            {   0,    0},  
            {4000,    0},  
            {4000, 2000}, 
            {2000, 2000}, 
            {2000, 4000}, 
            {   0, 4000}
        };
        pmg::pattern::PieceVariant variant1(1, 1, 0, false, 1, false, 0, 0, polygon1);

        std::vector<pmg::geometry::Point64> polygon2 = {
            {1000,    0},  
            {2000,    0},  
            {2000, 2000}, 
            {   0, 2000}, 
            {   0, 1000}, 
            {1000, 1000}
        };
        pmg::pattern::PieceVariant variant2(2, 2, 0, false, 1, false, 0, 0, polygon2);

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildNFPfor(variant1, variant2);

        Clipper2Lib::Paths64 expected = {{
            { 4000, -2000},  
            { 4000,  1000},  
            { 3000,  1000}, 
            { 3000,  2000},
            { 2000,  2000},
            { 2000,  3000},
            { 1000,  3000},
            { 1000,  4000},
            {-2000,  4000},
            {-2000, -2000}
        }};

        REQUIRE(result == expected);
    }

}