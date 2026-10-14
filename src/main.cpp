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
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 2 }
    };

    // Create points for pattern pieces
    // Trapezium 
    std::vector<geometry::Point64> trapeziumPts = {
        {      0, 1500000},  
        {9000000,       0},  
        {9000000, 4000000},  
        {      0, 2500000} 
    };

    // Square
    std::vector<geometry::Point64> squarePts = {
        {      0,       0}, 
        {6000000,       0}, 
        {6000000, 6000000}, 
        {      0, 6000000}  
    };

    // Rectangle
    std::vector<geometry::Point64> rectPts = {
        {      0,       0},  
        {4200000,       0},  
        {4200000, 1700000}, 
        {      0, 1700000}  
    };


    // Octagon
    constexpr double sideA = 1200000.0;
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
        {1500000,       0}, 
        {6500000,  500000}, 
        {6500000, 3000000}, 
        {1500000, 3500000},  
        {      0, 1500000}, 
        {      0, 1000000}  
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


