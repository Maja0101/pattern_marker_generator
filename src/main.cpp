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
#include "geometry/svg_exporter.hpp"
#include "geometry/geometry_types.hpp"
#include "pattern/pattern_piece.hpp"
#include "pattern/shapes.hpp"
#include "nesting/nested_item.hpp"
#include "nesting/basic_algorithm.hpp"
#include "sample_input.hpp"

using namespace pmg;

int main()
{
    logging::initialize();

    SPDLOG_INFO("Starting Pattern Marker Generator...");

    std::vector<pattern::VariantConstraint> defaultConstraints = {
        { .rotationAngle = 30, .isMirrored = false, .requiredQuantity = 1 },
        { .rotationAngle = 45, .isMirrored = false, .requiredQuantity = 1 },
        { .rotationAngle = 60, .isMirrored = false, .requiredQuantity = 1 }
    };

    std::vector<pattern::PatternPiece> samplePieces = pmg::samples::getSimpleSamplePieces(defaultConstraints);

    std::vector<geometry::Point64> fabricPts = {
        {       0,         0},  
        {35000000,        0},  
        {35000000, 15000000}, 
        {       0, 15000000}  
    };
    pattern::Fabric fabric(fabricPts);

    nesting::BasicAlgorithm alg(fabric);
    alg.execute(samplePieces);

    return 0;
}


