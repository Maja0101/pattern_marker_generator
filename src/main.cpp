#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>

#include <spdlog/spdlog.h>
#include <clipper2/clipper.h>

#include "utils/logger_setup.hpp"
#include "utils/timer.hpp"
#include "geometry/geometry_types.hpp"
#include "pattern/pattern_piece.hpp"
#include "pattern/shapes.hpp"
#include "nesting/nested_item.hpp"
#include "nesting/basic_algorithm.hpp"
#include "nesting/nesting_result.hpp"
#include "sample_input.hpp"

using namespace pmg;

int main()
{
    logging::initialize();

    SPDLOG_INFO("Starting Pattern Marker Generator...");

    std::vector<pattern::VariantConstraint> defaultConstraints = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
        { .rotationAngle = 45, .isMirrored = false, .requiredQuantity = 1, .twoWay = true }
    };

    int64_t sa = 10000; 
    int64_t ca =  5000; 

    std::vector<pattern::PatternPiece> samplePieces = 
        pmg::samples::getSimpleSamplePieces(defaultConstraints, sa, ca);

    std::vector<geometry::Point64> fabricPts = {
        {      0,       0},  
        {4500000,       0},  
        {4500000, 1500000}, 
        {      0, 1500000}  
    };

    const std::vector<pmg::geometry::Point64> lShapePts = {
        {      0,       0},
        {4000000,       0},
        {4000000, 1000000},
        {1000000, 1000000},
        {1000000, 1500000},
        {      0, 1500000}
    };

    pattern::Fabric fabric(fabricPts);

    nesting::BasicAlgorithm alg(1500000, 4500000);
    // nesting::BasicAlgorithm alg;

    nesting::NestingResult result =  alg.execute(samplePieces);

    SPDLOG_INFO(std::format("The nesting was{} finished", (result.wasSuccessful ? "" : "n't")));
    SPDLOG_INFO(std::format("The minmal length of fabric needed is {} RMT", (std::ceil(static_cast<double>(result.minLength) / 10000) / 100.0)));

    // for (const auto& ip : result.positions) {
    //     SPDLOG_INFO(std::format("Item no. {} (angle: {}, mirrored: {}, flipped 180: {}) was placed: ({}, {})", ip.id, ip.rotationAngle, ip.isMirrored, ip.isFlipped180, ip.x, ip.y));
    // }

    return 0;
}


