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
#include "nesting/nested_item.hpp"

using namespace pmg;

int main()
{
    logging::initialize();

    SPDLOG_INFO("Starting Pattern Marker Generator...");

    bool result;

    {
        Timer timer;

        const std::vector<pattern::VariantConstraint> defaultConstraints = {
            { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1 }
        };

        std::vector<geometry::Point64> trapeziumPts = {
            {     0,      0},
            {     0, -40000},
            {-90000, -25000},
            {-90000, -15000}
        };
        auto pieceTrapezium = std::make_shared<pattern::PatternPiece>(
            1, trapeziumPts, defaultConstraints
        );

        std::vector<geometry::Point64> squarePts = {
            {    0,      0},
            {60000,      0},
            {60000, -60000},
            {    0, -60000}
        };
        auto pieceSquare = std::make_shared<pattern::PatternPiece>(
            2, squarePts, defaultConstraints
        );

        std::vector<geometry::Point64> rectPts = {
            {    0,      0},
            {42000,      0},
            {42000, -17000},
            {    0, -17000}
        };
        auto pieceRect = std::make_shared<pattern::PatternPiece>(
            3, rectPts, defaultConstraints
        );

        constexpr double sideA = 12000.0;
        const double R = sideA / (2.0 * std::sin(M_PI / 8.0));
        std::vector<geometry::Point64> rawOctagonPts;

        double minX = 1e9, maxY = -1e9;
        for (int i = 0; i < 8; ++i) {
            double angle = (M_PI / 8.0) + i * (M_PI / 4.0);
            double x = R * std::cos(angle);
            double y = R * std::sin(angle);
            minX = std::min(minX, x);
            maxY = std::max(maxY, y); 
            auto px = static_cast<int64_t>(std::round(x));
            auto py = static_cast<int64_t>(std::round(y));
            rawOctagonPts.push_back({px, py});
        }

        std::vector<geometry::Point64> octagonPts;
        int64_t offsetX = std::round(minX);
        int64_t offsetY = std::round(maxY);

        for (const auto& pt : rawOctagonPts) {
            octagonPts.push_back({pt.x - offsetX, pt.y - offsetY});
        }

        auto pieceOctagon = std::make_shared<pattern::PatternPiece>(
            4, octagonPts, defaultConstraints
        );

        std::vector<geometry::Point64> dualTrapeziumPts = {
            {     0,  -5000},
            {     0, -30000},
            {-50000, -35000},
            {-65000, -25000},
            {-65000, -20000},
            {-50000,      0}
        };
        auto pieceDualTrapezium = std::make_shared<pattern::PatternPiece>(
            5, dualTrapeziumPts, defaultConstraints
        );

        std::vector<nesting::NestedItem> nestedItems;

        nesting::NestedItem trapezium(pieceTrapezium->getVariant(0, false));
        trapezium.lockInPosition(100000, 130000);
        nestedItems.emplace_back(trapezium);

        nesting::NestedItem square(pieceSquare->getVariant(0, false));
        square.lockInPosition(80000, 80000);
        nestedItems.emplace_back(square);

        nesting::NestedItem rect(pieceRect->getVariant(0, false));
        rect.lockInPosition(160000, 40000);
        nestedItems.emplace_back(rect);

        nesting::NestedItem octagon(pieceOctagon->getVariant(0, false));
        octagon.lockInPosition(220000, 50000);
        nestedItems.emplace_back(octagon);

        nesting::NestedItem dualTrapezium(pieceDualTrapezium->getVariant(0, false));
        dualTrapezium.lockInPosition(330000, 90000);
        nestedItems.emplace_back(dualTrapezium);

        for (const auto& piece : nestedItems) {
            SPDLOG_INFO(piece.toString());
        }

        result = pmg::geometry::SvgExporter::exportLayout(
            "debug_output.svg", 350, 150, nestedItems
        );

        SPDLOG_INFO("Placed geometric pieces");
    }

    return result ? 0 : 1;

    // return 1;
}


