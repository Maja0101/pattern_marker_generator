#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include "utils/logger_setup.hpp"
#include "utils/timer.hpp"
#include "geometry/svg_exporter.hpp"
#include "geometry/geometry_types.hpp"
#include "pattern/pattern_piece.hpp"
#include "nesting/placed_instance.hpp"

#include <spdlog/spdlog.h>
#include <clipper2/clipper.h>

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>

using namespace pmg;

int main()
{
    logging::initialize();

    SPDLOG_INFO("Starting Pattern Marker Generator...");

    // bool result;

    // {
    //     Timer timer;

    //     const std::vector<pattern::VariantConstraint> defaultConstraints = {
    //         { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1 }
    //     };

    //     std::vector<geometry::Point64> trapeziumPts = {
    //         {    0,      0},
    //         {40000,      0},
    //         {25000, -90000},
    //         {15000, -90000}
    //     };
    //     auto pieceTrapezium = std::make_shared<pattern::PatternPiece>(
    //         1, trapeziumPts, defaultConstraints
    //     );

    //     std::vector<geometry::Point64> squarePts = {
    //         {    0,      0},
    //         {60000,      0},
    //         {60000, -60000},
    //         {    0, -60000}
    //     };
    //     auto pieceSquare = std::make_shared<pattern::PatternPiece>(
    //         2, squarePts, defaultConstraints
    //     );

    //     std::vector<geometry::Point64> rectPts = {
    //         {    0,      0},
    //         {42000,      0},
    //         {42000, -17000},
    //         {    0, -17000}
    //     };
    //     auto pieceRect = std::make_shared<pattern::PatternPiece>(
    //         3, rectPts, defaultConstraints
    //     );

    //     constexpr double sideA = 12000.0;
    //     const double R = sideA / (2.0 * std::sin(M_PI / 8.0));
    //     std::vector<geometry::Point64> rawOctagonPts;

    //     double minX = 1e9, maxY = -1e9;
    //     for (int i = 0; i < 8; ++i) {
    //         double angle = (M_PI / 8.0) + i * (M_PI / 4.0);
    //         double x = R * std::cos(angle);
    //         double y = R * std::sin(angle);
    //         minX = std::min(minX, x);
    //         maxY = std::max(maxY, y); 
    //         auto px = static_cast<int64_t>(std::round(x));
    //         auto py = static_cast<int64_t>(std::round(y));
    //         rawOctagonPts.push_back({px, py});
    //     }

    //     std::vector<geometry::Point64> octagonPts;
    //     int64_t offsetX = std::round(minX);
    //     int64_t offsetY = std::round(maxY);

    //     for (const auto& pt : rawOctagonPts) {
    //         octagonPts.push_back({pt.x - offsetX, pt.y - offsetY});
    //     }

    //     auto pieceOctagon = std::make_shared<pattern::PatternPiece>(
    //         4, octagonPts, defaultConstraints
    //     );

    //     std::vector<geometry::Point64> dualTrapeziumPts = {
    //         { 5000,      0}, // Bottom-Left
    //         {30000,      0}, // Bottom-Right
    //         {35000, -50000}, // Joint Right
    //         {25000, -65000}, // Top-Right
    //         {20000, -65000}, // Top-Left
    //         {    0, -50000}  // Joint Left
    //     };
    //     auto pieceDualTrapezium = std::make_shared<pattern::PatternPiece>(
    //         5, dualTrapeziumPts, defaultConstraints
    //     );

    //     std::vector<pmg::nesting::PlacedInstance> placedInstances;

    //     placedInstances.emplace_back(
    //         geometry::Point64{20000, 110000}, pieceTrapezium->getVariant(0, false)
    //     );
    //     placedInstances.emplace_back(
    //         geometry::Point64{80000, 80000}, pieceSquare->getVariant(0, false)
    //     );
    //     placedInstances.emplace_back(
    //         geometry::Point64{160000, 40000}, pieceRect->getVariant(0, false)
    //     );
    //     placedInstances.emplace_back(
    //         geometry::Point64{220000, 50000}, pieceOctagon->getVariant(0, false)
    //     );
    //     placedInstances.emplace_back(
    //         geometry::Point64{270000, 85000}, pieceDualTrapezium->getVariant(0, false)
    //     );

    //     for (const auto& piece : placedInstances) {
    //         SPDLOG_INFO(piece.toString());
    //     }

    //     result = pmg::geometry::SvgExporter::exportLayout(
    //         "debug_output.svg", 350, 150, placedInstances
    //     );

    //     SPDLOG_INFO("Placed geometric pieces");
    // }

    // return result ? 0 : 1;

    Clipper2Lib::Paths64 subject, clip, solution; 
    subject.push_back({ Clipper2Lib::MakePath({200,100, 20,158, 130,4, 130,196, 20,42}) }); 
    clip.push_back({ Clipper2Lib::MakePath({196,126, 8,136, 154,16, 104,200, 38,24}) }); 
    solution = Clipper2Lib::Intersect(subject, clip, Clipper2Lib::FillRule::NonZero); 

    std::cout << "Intersection path count: " << solution.size() << std::endl;
    for (const Clipper2Lib::Path64& path : solution) {
        std::cout << "Path vertex count: " << path.size() << std::endl;
        for (const Clipper2Lib::Point64& pt : path) {
            std::cout << "  Point: (" << pt.x << ", " << pt.y << ")\n";
        }
    }

    return 0;
}


