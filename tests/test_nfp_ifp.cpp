#include <catch2/catch_amalgamated.hpp>
#include <clipper2/clipper.h>
#include <spdlog/spdlog.h>

#include "geometry/geometry_types.hpp"
#include "geometry/nfp_ifp_calculations.hpp"
#include "geometry/nfp_key.hpp"
#include "pattern/shapes.hpp"

TEST_CASE("Calculating IFP", "[IFP]") {
    SECTION("Rectangular fabric") {
        std::vector<pmg::geometry::Point64> fabricPts = {
            {   0,    0},  
            {6000,    0},  
            {6000, 6000}, 
            {   0, 6000} 
        };
        pmg::pattern::Fabric fabric(fabricPts, 90);

        std::vector<pmg::geometry::Point64> polygon = {
            {2000,    0},  
            {4000,    0},  
            {4000, 4000}, 
            {   0, 4000}, 
            {   0, 2000}, 
            {2000, 2000}
        };
        pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, false, 0, 0, polygon);

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildIFPfor(fabric, variant, false);

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
        pmg::pattern::Fabric fabric(fabricPts, 90);

        std::vector<pmg::geometry::Point64> polygon = {
            {   0,    0},  
            {7000,    0},  
            {7000, 6000}, 
            {   0, 6000} 
        };
        pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, false, 0, 0, polygon);

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildIFPfor(fabric, variant, false);

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
        pmg::pattern::Fabric fabric(fabricPts, 90);

        std::vector<pmg::geometry::Point64> polygon = {
            {   0,    0},  
            {2000,    0},  
            {2000, 2000}, 
            {   0, 2000} 
        };
        pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, false, 0, 0, polygon);

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildIFPfor(fabric, variant, false);

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
        pmg::pattern::Fabric fabric(fabricPts, 90);

        std::vector<pmg::geometry::Point64> polygon = {
            {2000,     0},  
            {4000,     0},  
            {4000,  4000}, 
            {   0,  4000},
            {   0,  2000}
        };
        pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, false, 0, 0, polygon);

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildIFPfor(fabric, variant, false);

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
        pmg::pattern::Fabric fabric(fabricPts, 90);

        std::vector<pmg::geometry::Point64> polygon = {
            {2000,     0},  
            {4000,     0},  
            {4000,  4000}, 
            {   0,  4000},
            {   0,  2000},
            {2000,  2000}
        };
        pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, false, 0, 0, polygon);

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildIFPfor(fabric, variant, false);

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
        pmg::pattern::Fabric fabric(fabricPts, 90);

        std::vector<pmg::geometry::Point64> polygon = {
            {1000,     0},  
            {2000,     0},  
            {2000,  2000}, 
            {   0,  2000},
            {   0,  1000},
            {1000,  1000}
        };
        pmg::pattern::PieceVariant variant(1, 1, 0, false, 1, false, 0, 0, polygon);

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildIFPfor(fabric, variant, false);

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

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildNFPfor(variant1, variant2, false, false);

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

        Clipper2Lib::Paths64 result = pmg::geometry::calculations::utils::buildNFPfor(variant1, variant2, false, false);

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

TEST_CASE("Creating cache for two-way pieces", "[NFP][IFP]") {
    std::vector<pmg::geometry::Point64> fabricPts = {
        {    0,     0},  
        {60000,     0},  
        {60000, 60000}, 
        {    0, 60000} 
    };
    pmg::pattern::Fabric fabric(fabricPts, 90);

    std::vector<pmg::geometry::Point64> boundary = {
        {    0,     0},
        {40000,     0},
        {40000, 10000},
        {    0, 40000}
    };

    SECTION("Two single way pieces") {
        pmg::pattern::PieceVariant variant1(1, 1, 0, false, 1, false, 0, 0, boundary);
        pmg::pattern::PieceVariant variant2(2, 1, 0, false, 1, false, 0, 0, boundary);
        std::vector<pmg::pattern::PieceVariant> variants = { variant1, variant2 };

        SECTION("IFP cache") {
            std::unordered_map<int64_t, Clipper2Lib::Paths64> cache = pmg::geometry::calculations::calculateIFPCache(fabric, variants);
            
            REQUIRE(cache.size() == 2);
            REQUIRE(cache.find(variant1.getGlobalId(false)) != cache.end());
            REQUIRE(cache.find(variant2.getGlobalId(false)) != cache.end());
        }

        SECTION("NFP cache") {
            std::unordered_map<pmg::geometry::NFPKey, Clipper2Lib::Paths64> cache = pmg::geometry::calculations::calculateNFPCache(variants);
            
            REQUIRE(cache.size() == 4);
            REQUIRE(cache.find({ variant1.getGlobalId(false), variant1.getGlobalId(false)}) != cache.end());
            REQUIRE(cache.find({ variant1.getGlobalId(false), variant2.getGlobalId(false)}) != cache.end());
            REQUIRE(cache.find({ variant2.getGlobalId(false), variant1.getGlobalId(false)}) != cache.end());
            REQUIRE(cache.find({ variant2.getGlobalId(false), variant2.getGlobalId(false)}) != cache.end());
        }
    }
    
    SECTION("Two two way pieces") {
        pmg::pattern::PieceVariant variant1(1, 1, 0, false, 1, true, 0, 0, boundary);
        pmg::pattern::PieceVariant variant2(2, 1, 0, false, 1, true, 0, 0, boundary);
        std::vector<pmg::pattern::PieceVariant> variants = { variant1, variant2 };

        SECTION("IFP cache") {
            std::unordered_map<int64_t, Clipper2Lib::Paths64> cache = pmg::geometry::calculations::calculateIFPCache(fabric, variants);

            REQUIRE(cache.size() == 4);
            REQUIRE(cache.find(variant1.getGlobalId(false)) != cache.end());
            REQUIRE(cache.find(variant1.getGlobalId(true)) != cache.end());
            REQUIRE(cache.find(variant2.getGlobalId(false)) != cache.end());
            REQUIRE(cache.find(variant2.getGlobalId(true)) != cache.end());
        }

        SECTION("NFP cache") {
            std::unordered_map<pmg::geometry::NFPKey, Clipper2Lib::Paths64> cache = pmg::geometry::calculations::calculateNFPCache(variants);
            
            REQUIRE(cache.size() == 16);
            REQUIRE(cache.find({ variant1.getGlobalId(false), variant1.getGlobalId(false)}) != cache.end());
            REQUIRE(cache.find({ variant1.getGlobalId(true), variant1.getGlobalId(true)}) != cache.end());
            REQUIRE(cache.find({ variant1.getGlobalId(false), variant1.getGlobalId(true)}) != cache.end());
            REQUIRE(cache.find({ variant1.getGlobalId(true), variant1.getGlobalId(false)}) != cache.end());

            REQUIRE(cache.find({ variant2.getGlobalId(false), variant2.getGlobalId(false)}) != cache.end());
            REQUIRE(cache.find({ variant2.getGlobalId(true), variant2.getGlobalId(true)}) != cache.end());
            REQUIRE(cache.find({ variant2.getGlobalId(false), variant2.getGlobalId(true)}) != cache.end());
            REQUIRE(cache.find({ variant2.getGlobalId(true), variant2.getGlobalId(false)}) != cache.end());

            REQUIRE(cache.find({ variant1.getGlobalId(false), variant2.getGlobalId(false)}) != cache.end());
            REQUIRE(cache.find({ variant1.getGlobalId(true), variant2.getGlobalId(false)}) != cache.end());
            REQUIRE(cache.find({ variant1.getGlobalId(true), variant2.getGlobalId(true)}) != cache.end());
            REQUIRE(cache.find({ variant1.getGlobalId(false), variant2.getGlobalId(true)}) != cache.end());

            REQUIRE(cache.find({ variant2.getGlobalId(false), variant1.getGlobalId(false)}) != cache.end());
            REQUIRE(cache.find({ variant2.getGlobalId(true), variant1.getGlobalId(false)}) != cache.end());
            REQUIRE(cache.find({ variant2.getGlobalId(true), variant1.getGlobalId(true)}) != cache.end());
            REQUIRE(cache.find({ variant2.getGlobalId(false), variant1.getGlobalId(true)}) != cache.end());

        }
     }
}