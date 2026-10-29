#include <spdlog/spdlog.h>

#include "sample_input.hpp"
#include "utils/config.hpp"
#include "geometry/geometry_types.hpp"

namespace pmg::samples {
    std::vector<pattern::PatternPiece> getSimpleSamplePieces(
        std::vector<pattern::VariantConstraint> defaultConstraints,
        int64_t seamAllowance, int64_t cuttingAllowance) {
            // Create points for pattern pieces
            // Trapezium 
            std::vector<geometry::Point64> trapeziumPts = {
                {     0, 150000},  
                {900000,      0},  
                {900000, 400000},  
                {     0, 250000} 
            };

            // Square
            std::vector<geometry::Point64> squarePts = {
                {     0,      0}, 
                {600000,      0}, 
                {600000, 600000}, 
                {     0, 600000}  
            };

            // Rectangle
            std::vector<geometry::Point64> rectPts = {
                {     0,      0},  
                {420000,      0},  
                {420000, 170000}, 
                {     0, 170000}  
            };


            // Octagon
            constexpr double sideA = 120000.0;
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
                {150000,      0}, 
                {650000,  50000}, 
                {650000, 300000}, 
                {150000, 350000},  
                {     0, 150000}, 
                {     0, 100000}  
            };

            // Small Square
            std::vector<geometry::Point64> smallSquarePts = {
                {    0,     0}, 
                {60000,     0}, 
                {60000, 60000}, 
                {    0, 60000}   
            };

            std::vector<pattern::VariantConstraint> smallSquareConstraints = {
                { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 100 , .twoWay = false }
            };

            // L shape
            const std::vector<pmg::geometry::Point64> lShapePts = {
                {     0,      0},
                {150000,      0},
                {150000, 100000},
                {100000, 100000},
                {100000, 250000},
                {     0, 250000}
            };

            // inverted C shape
            const std::vector<pmg::geometry::Point64> cShapePts = {
                {     0,      0},
                {200000,      0},
                {200000, 400000},
                {     0, 400000},
                {     0, 350000},
                {100000, 350000},
                {100000,  50000},
                {     0,  50000}
            };

            std::vector<pattern::VariantConstraint> otherConstraints;
            for (auto& c : defaultConstraints) {
                pattern::VariantConstraint constraint1 = { .rotationAngle = c.rotationAngle, .isMirrored = false, .requiredQuantity = c.requiredQuantity * 3, .twoWay = c.twoWay };
                pattern::VariantConstraint constraint2 = { .rotationAngle = c.rotationAngle  + 180, .isMirrored = false, .requiredQuantity = c.requiredQuantity * 3, .twoWay = c.twoWay };
                otherConstraints.push_back(constraint1);
                otherConstraints.push_back(constraint2);
            }

            // Add them to vector of pattern pieces
            std::vector<pattern::PatternPiece> patternPieces;
            patternPieces.reserve(8);
            patternPieces.emplace_back(1, trapeziumPts, defaultConstraints, seamAllowance, cuttingAllowance);
            patternPieces.emplace_back(2, squarePts, defaultConstraints, seamAllowance, cuttingAllowance);
            patternPieces.emplace_back(3, rectPts, defaultConstraints, seamAllowance, cuttingAllowance);
            patternPieces.emplace_back(4, octagonPts, defaultConstraints, seamAllowance, cuttingAllowance);
            patternPieces.emplace_back(5, doubleTrapeziumPts, defaultConstraints, seamAllowance, cuttingAllowance);
            patternPieces.emplace_back(6, smallSquarePts, smallSquareConstraints, seamAllowance, cuttingAllowance);
            patternPieces.emplace_back(7, lShapePts, otherConstraints, seamAllowance, cuttingAllowance);
            patternPieces.emplace_back(8, cShapePts, otherConstraints, seamAllowance, cuttingAllowance);


            if constexpr (config::DEBUG_MODE_DETAILED) {
                for (const auto& piece : patternPieces) {
                    SPDLOG_INFO(piece.toString());
                }
            }

            return patternPieces;
        }
} // namespace pmg::samples