#include <catch2/catch_amalgamated.hpp>

#include "pattern/shapes.hpp"
#include "geometry/geometry_types.hpp"
#include "geometry/calculations.hpp"

TEST_CASE("Geometry initialization with two-way pieces", "[path][rect][two-way]") {
    SECTION("Concave shape geometry initialization") {
        std::vector<pmg::geometry::Point64> boundary = {
            {    0,     0},
            {40000,     0},
            {40000, 10000},
            {20000, 10000},
            {20000, 50000},
            {    0, 50000}
        };

        std::vector<Clipper2Lib::Path64> expectedDecomposedShape = {
            {
                {    0, 50000},
                {    0,     0},
                {20000, 10000},
                {20000, 50000}
            },
            {
                {40000,     0},
                {40000, 10000},
                {20000, 10000},
                {    0,     0}
            }
        };

        SECTION("Valid geometry initialization with one-way piece") {
            pmg::pattern::PieceVariant testVariant(1, 1, 0, false, 1, false, 0, 0, boundary);

            REQUIRE_FALSE(testVariant.getIsTwoWay());
            REQUIRE(testVariant.getGlobalId(false) == 4097);
            REQUIRE(testVariant.getGlobalId(true) == 4097);
            REQUIRE(testVariant.getBoundary(false) == boundary);
            REQUIRE(testVariant.getBoundary(true) == boundary);
            REQUIRE(testVariant.getPath(false) == pmg::geometry::calculations::toClipperPath(boundary));
            REQUIRE(testVariant.getPath(true) == pmg::geometry::calculations::toClipperPath(boundary));
            REQUIRE(testVariant.getDecomposedShape(false) == expectedDecomposedShape);
            REQUIRE(testVariant.getDecomposedShape(true) == expectedDecomposedShape);
        }

        SECTION("Valid geometry initialization with two-way piece") {
            pmg::pattern::PieceVariant testVariant(1, 1, 0, false, 1, true, 0, 0, boundary);

            std::vector<pmg::geometry::Point64> expectedFlippedBoundary = {
                {40000, 50000},
                {    0, 50000},
                {    0, 40000},
                {20000, 40000},
                {20000,     0},
                {40000,     0}
            };

            std::vector<Clipper2Lib::Path64> expectedFlippedDecomposedShape = {
                {
                    {40000,     0},
                    {40000, 50000},
                    {20000, 40000},
                    {20000,     0}
                },
                {
                    {    0, 50000},
                    {    0, 40000},
                    {20000, 40000},
                    {40000, 50000}
                }
            };

            REQUIRE(testVariant.getIsTwoWay());
            REQUIRE(testVariant.getGlobalId(false) == 4097);
            REQUIRE(testVariant.getGlobalId(true) == -9223372036854771711);
            REQUIRE(testVariant.getBoundary(false) == boundary);
            REQUIRE(testVariant.getBoundary(true) == expectedFlippedBoundary);
            REQUIRE(testVariant.getPath(false) == pmg::geometry::calculations::toClipperPath(boundary));
            REQUIRE(testVariant.getPath(true) == pmg::geometry::calculations::toClipperPath(expectedFlippedBoundary));
            REQUIRE(testVariant.getDecomposedShape(false) == expectedDecomposedShape);
            REQUIRE(testVariant.getDecomposedShape(true) == expectedFlippedDecomposedShape);
        }
    }

    SECTION("Valid geometry initialization with two-way piece that is symmetrical") {
        std::vector<pmg::geometry::Point64> boundary = {
            {    0,     0},
            {40000,     0},
            {40000, 10000},
            {    0, 10000}
        };

        pmg::pattern::PieceVariant testVariant(1, 1, 0, false, 1, true, 0, 0, boundary);

        REQUIRE_FALSE(testVariant.getIsTwoWay());
        REQUIRE(testVariant.getGlobalId(false) == 4097);
        REQUIRE(testVariant.getGlobalId(true) == 4097);
        REQUIRE(testVariant.getBoundary(false) == boundary);
        REQUIRE(testVariant.getBoundary(true) == boundary);
        REQUIRE(testVariant.getPath(false) == pmg::geometry::calculations::toClipperPath(boundary));
        REQUIRE(testVariant.getPath(true) == pmg::geometry::calculations::toClipperPath(boundary));
        REQUIRE(testVariant.getDecomposedShape(false)[0] == pmg::geometry::calculations::toClipperPath(boundary));
        REQUIRE(testVariant.getDecomposedShape(true)[0] == pmg::geometry::calculations::toClipperPath(boundary));
    }
}