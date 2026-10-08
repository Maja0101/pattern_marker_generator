#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include "utils/logger_setup.hpp"
#include "utils/timer.hpp"
#include "geometry/svg_exporter.hpp"
#include "geometry/geometry_types.hpp"
#include "pattern/pattern_piece.hpp"
#include "nesting/placed_instance.hpp"

#include <spdlog/spdlog.h>

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

    bool result;

    {
        Timer timer;

        constexpr int64_t MM = 1000;

        const std::vector<pattern::VariantConstraint> defaultConstraints = {
            { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1 }
        };

        std::vector<geometry::Point64> trapeziumPts = {
            { 0 * MM,   0 * MM},
            {40 * MM,   0 * MM},
            {25 * MM, -90 * MM},
            {15 * MM, -90 * MM}
        };
        auto pieceTrapezium = std::make_shared<pattern::PatternPiece>(
            1, trapeziumPts, defaultConstraints
        );

        std::vector<geometry::Point64> squarePts = {
            { 0 * MM,   0 * MM},
            {60 * MM,   0 * MM},
            {60 * MM, -60 * MM},
            { 0 * MM, -60 * MM}
        };
        auto pieceSquare = std::make_shared<pattern::PatternPiece>(
            2, squarePts, defaultConstraints
        );

        std::vector<geometry::Point64> rectPts = {
            { 0 * MM,   0 * MM},
            {42 * MM,   0 * MM},
            {42 * MM, -17 * MM},
            { 0 * MM, -17 * MM}
        };
        auto pieceRect = std::make_shared<pattern::PatternPiece>(
            3, rectPts, defaultConstraints
        );

        constexpr double sideA = 12.0;
        const double R = sideA / (2.0 * std::sin(M_PI / 8.0));
        std::vector<geometry::Point64> rawOctagonPts;

        double minX = 1e9, maxY = -1e9;
        for (int i = 0; i < 8; ++i) {
            double angle = (M_PI / 8.0) + i * (M_PI / 4.0);
            double x = R * std::cos(angle);
            double y = R * std::sin(angle);
            minX = std::min(minX, x);
            maxY = std::max(maxY, y); 
            auto px = static_cast<int64_t>(std::round(x * MM));
            auto py = static_cast<int64_t>(std::round(y * MM));
            rawOctagonPts.push_back({px, py});
        }

        std::vector<geometry::Point64> octagonPts;
        int64_t offsetX = std::round(minX * MM);
        int64_t offsetY = std::round(maxY * MM);

        for (const auto& pt : rawOctagonPts) {
            octagonPts.push_back({pt.x - offsetX, pt.y - offsetY});
        }

        auto pieceOctagon = std::make_shared<pattern::PatternPiece>(
            4, octagonPts, defaultConstraints
        );

        std::vector<geometry::Point64> dualTrapeziumPts = {
            { 5 * MM,   0 * MM}, // Bottom-Left
            {30 * MM,   0 * MM}, // Bottom-Right
            {35 * MM, -50 * MM}, // Joint Right
            {25 * MM, -65 * MM}, // Top-Right
            {20 * MM, -65 * MM}, // Top-Left
            { 0 * MM, -50 * MM}  // Joint Left
        };
        auto pieceDualTrapezium = std::make_shared<pattern::PatternPiece>(
            5, dualTrapeziumPts, defaultConstraints
        );

        std::vector<pmg::nesting::PlacedInstance> placedInstances;

        placedInstances.emplace_back(
            geometry::Point64{20 * MM, 110 * MM}, pieceTrapezium->getVariant(0, false)
        );
        placedInstances.emplace_back(
            geometry::Point64{80 * MM, 80 * MM}, pieceSquare->getVariant(0, false)
        );
        placedInstances.emplace_back(
            geometry::Point64{160 * MM, 40 * MM}, pieceRect->getVariant(0, false)
        );
        placedInstances.emplace_back(
            geometry::Point64{220 * MM, 50 * MM}, pieceOctagon->getVariant(0, false)
        );
        placedInstances.emplace_back(
            geometry::Point64{270 * MM, 85 * MM}, pieceDualTrapezium->getVariant(0, false)
        );

        for (const auto& piece : placedInstances) {
            SPDLOG_INFO(piece.toString());
        }

        result = pmg::geometry::SvgExporter::exportLayout(
            "debug_output.svg", 350, 150, placedInstances
        );

        SPDLOG_INFO("Placed geometric pieces");
    }

    return result ? 0 : 1;

    // return 0;
}


