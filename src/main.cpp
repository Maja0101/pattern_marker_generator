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

using namespace pmg;

std::vector<pattern::PatternPiece> getSamplePieces() {
    // Default constraints
    const std::vector<pattern::VariantConstraint> defaultConstraints = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1 }
    };

    // Create points for pattern pieces
    // Trapezium 
    std::vector<geometry::Point64> trapeziumPts = {
        {    0, 15000},  
        {90000,     0},  
        {90000, 40000},  
        {    0, 25000} 
    };

    // Square
    std::vector<geometry::Point64> squarePts = {
        {    0,     0}, 
        {60000,     0}, 
        {60000, 60000}, 
        {    0, 60000}  
    };

    // Rectangle
    std::vector<geometry::Point64> rectPts = {
        {    0,     0},  
        {42000,     0},  
        {42000, 17000}, 
        {    0, 17000}  
    };


    // Octagon
    constexpr double sideA = 12000.0;
    const double R = sideA / (2.0 * std::sin(M_PI / 8.0));
    std::vector<geometry::Point64> rawOctagonPts;

    double minX = 1e9, minY = 1e9;
    for (int i = 0; i < 8; ++i) {
        double angle = (5.0 * M_PI / 4.0) + i * (M_PI / 4.0);
        double x = R * std::cos(angle);
        double y = R * std::sin(angle);
        minX = std::min(minX, x);
        minY = std::min(minY, y);
        
        auto px = static_cast<int64_t>(std::round(x));
        auto py = static_cast<int64_t>(std::round(y));
        rawOctagonPts.push_back({px, py});
    }

    std::vector<geometry::Point64> octagonPts;
    int64_t offsetX = std::round(minX);
    int64_t offsetY = std::round(minY);

    for (const auto& pt : rawOctagonPts) {
        octagonPts.push_back({pt.x - offsetX, pt.y - offsetY});
    }

    // Double Trapezium 
    std::vector<geometry::Point64> doubleTrapeziumPts = {
        {15000,     0}, 
        {65000,  5000}, 
        {65000, 30000}, 
        {15000, 35000},  
        {    0, 15000}, 
        {    0, 10000}  
    };

    // Add them to vector of pattern pieces
    std::vector<pattern::PatternPiece> patternPieces;
    patternPieces.reserve(5);
    patternPieces.emplace_back(1, trapeziumPts, defaultConstraints);
    patternPieces.emplace_back(2, squarePts, defaultConstraints);
    patternPieces.emplace_back(3, rectPts, defaultConstraints);
    patternPieces.emplace_back(4, octagonPts, defaultConstraints);
    patternPieces.emplace_back(5, doubleTrapeziumPts, defaultConstraints);

    for (const auto& piece : patternPieces) {
        SPDLOG_INFO(piece.toString());
    }

    return patternPieces;
}

int main()
{
    logging::initialize();

    SPDLOG_INFO("Starting Pattern Marker Generator...");

    std::vector<pattern::PatternPiece> samplePieces = getSamplePieces();

    std::vector<geometry::Point64> fabricPts = {
        {    0,       0},  
        {350000,      0},  
        {350000, 150000}, 
        {     0, 150000}  
    };
    pattern::Fabric fabric(fabricPts);

    nesting::BasicAlgorithm alg(fabric);
    alg.execute(samplePieces);

    // std::vector<nesting::NestedItem> nestedItems;

    // nesting::NestedItem trapezium(samplePieces[0].getVariant(0, false));
    // trapezium.lockInPosition(10000, 10000);
    // nestedItems.emplace_back(trapezium);

    // nesting::NestedItem square(samplePieces[1].getVariant(0, false));
    // square.lockInPosition(120000, 10000);
    // nestedItems.emplace_back(square);

    // nesting::NestedItem rect(samplePieces[2].getVariant(0, false));
    // rect.lockInPosition(10000, 60000);
    // nestedItems.emplace_back(rect);

    // nesting::NestedItem octagon(samplePieces[3].getVariant(0, false));
    // octagon.lockInPosition(60000, 60000);
    // nestedItems.emplace_back(octagon);

    // nesting::NestedItem dualTrapezium(samplePieces[4].getVariant(0, false));
    // dualTrapezium.lockInPosition(200000, 10000);
    // nestedItems.emplace_back(dualTrapezium);

    // for (const auto& piece : nestedItems) {
    //     SPDLOG_INFO(piece.toString());
    // }

    // bool result = pmg::geometry::SvgExporter::exportLayout(
    //     "debug_output.svg", 350, 150, nestedItems
    // );

    return 0;
}


