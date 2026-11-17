#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <vector>
#include <memory>
#include <iostream>

#include <spdlog/spdlog.h>
#include <clipper2/clipper.h>

#include "utils/logger_setup.hpp"
#include "utils/timer.hpp"
#include "utils/config.hpp"
#include "geometry/geometry_types.hpp"
#include "geometry/transformations.hpp"
#include "pattern/pattern_piece.hpp"
#include "pattern/shapes.hpp"
#include "nesting/nested_item.hpp"
#include "nesting/basic_algorithm.hpp"
#include "nesting/advanced_algorithm.hpp"
#include "nesting/nesting_result.hpp"
#include "sample_input.hpp"

using namespace pmg;

int main()
{
    logging::initialize();

    if constexpr (config::MAIN_DEBUG) {

        if constexpr (config::DEVELOPMENT) SPDLOG_INFO("Starting Pattern Marker Generator...");

        std::vector<pattern::VariantConstraint> defaultConstraints = {
            { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 45, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
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

        fabricPts = {
            {1551, 2735},
            { 230, 2569},
            {  54,   75},
            {2220,  166},
            {2573, 1729}
        };

        // pattern::Fabric fabric(fabricPts, 360, true);
        
        // nesting::BasicAlgorithm alg;
        
        // nesting::AdvancedAlgorithm alg(1500000, 4500000);

        fabricPts = {
            {501, 1352},
            {546, 477},
            {1540, 213},
            {1492, 1002},
            {1012, 1317}
        };

        // fabricPts = {
        //     {0, 1686},
        //     {59, 415},
        //     {1531, 0},
        //     {1424, 1211},
        //     {760, 1638}
        // };
        
        // pattern::Fabric fabric(1000, 750, true);
        pattern::Fabric fabric(fabricPts, 0, true);
        nesting::BasicAlgorithm alg(fabric);
        std::vector<pmg::pattern::PatternPiece> piecesToCheck = samples::getPiecesToCheck2();
        // for (auto& piece : piecesToCheck) {
        //     SPDLOG_WARN(piece.getId());
        //     for (auto& variant : piece.getVariants()) {
        //         SPDLOG_WARN(variant.toString());
        //         SPDLOG_WARN(variant.getReferencePoint().toString());
        //     }
        // }
        nesting::NestingResult result =  alg.execute(piecesToCheck);


        // pattern::Fabric fabric(1400, 4500, true);
        // nesting::AdvancedAlgorithm alg(fabric);
        // std::vector<pmg::pattern::PatternPiece> testPieces = samples::getTestPieces();
        // nesting::NestingResult result =  alg.execute(testPieces);


        if constexpr (config::DEVELOPMENT) SPDLOG_INFO(std::format("The nesting was{} finished", (result.wasSuccessful ? "" : "n't")));
        if constexpr (config::DEVELOPMENT) SPDLOG_INFO(std::format("The minmal length of fabric needed is {} mn", result.minLength));
        if constexpr (config::DEVELOPMENT) SPDLOG_INFO(std::format("Execution time: {} ms", result.executionTime));

        if constexpr (config::DEVELOPMENT) SPDLOG_INFO(std::format("Scraps: {}, {} smm {}% Fabric: {} mm, {} smm {}% Waste: {} smm {}%", 
            result.stats.usableScrapCount, result.stats.usableScrapsArea, result.stats.usableScrapsPercentage,
            result.stats.remainingFabricLength, result.stats.remainingFabricArea, result.stats.remainingFabricPercentage,
            result.stats.wasteArea, result.stats.wastePercentage)
        );

        std::vector<std::vector<geometry::Point64>> shapes = {
            {
                {109, 321},
                {34, 312},
                {30, 10},
                {107, 20},
                {158, 35},
                {145, 142},
                {146, 210},
                {153, 274},
                {165, 321},
                {153, 328},
                {109, 321}
            },
            {
                {370, 473},
                {384, 436},
                {386, 399},
                {380, 371},
                {373, 358},
                {361, 346},
                {362, 331},
                {410, 317},
                {442, 315},
                {533, 342},
                {533, 382},
                {525, 413},
                {496, 422},
                {480, 432},
                {455, 459},
                {433, 493},
                {370, 473}
            },
            {
                {235, 479},
                {173, 502},
                {165, 492},
                {153, 485},
                {137, 480},
                {117, 476},
                {57, 475},
                {45, 389},
                {44, 325},
                {151, 331},
                {198, 344},
                {229, 357},
                {220, 386},
                {220, 440},
                {235, 479}
            },
            {
                {499, 329},
                {445, 308},
                {457, 254},
                {463, 168},
                {463, 47},
                {465, 40},
                {504, 33},
                {577, 27},
                {576, 52},
                {559, 225},
                {555, 341},
                {499, 329}
            },
            {
                {165, 321},
                {180, 262},
                {188, 194},
                {187, 123},
                {173, 36},
                {247, 31},
                {298, 35},
                {293, 140},
                {292, 212},
                {305, 305},
                {276, 314},
                {256, 327},
                {238, 350},
                {165, 321}
            },
            {
                {358, 325},
                {335, 312},
                {314, 304},
                {329, 214},
                {330, 132},
                {327, 35},
                {384, 28},
                {425, 29},
                {463, 35},
                {445, 141},
                {435, 234},
                {439, 302},
                {434, 309},
                {390, 315},
                {358, 325}
            }
        };

        if constexpr (config::DEVELOPMENT) {
            for (const auto& ip : result.positions) {
                // SPDLOG_INFO(std::format("Item no. {} (angle: {}, mirrored: {}, flipped 180: {}) position delta: ({}, {})", ip.id, ip.rotationAngle, ip.isMirrored, ip.isFlipped180, ip.x, ip.y));
                std::cout << std::format("Item no. {} (angle: {}, mirrored: {}, flipped 180: {}) position delta: ({}, {})", ip.id, ip.rotationAngle, ip.isMirrored, ip.isFlipped180, ip.x, ip.y) << "\n";
                std::cout << "original shape" << std::endl;
                for (auto& point : shapes[ip.id-7]) {
                    std::cout << point.toString() << " ";
                }
                std::cout << "\n";
                auto ts = geometry::calculations::transformPolygon(shapes[ip.id-7], ip.rotationAngle, false, false, false);
                std::cout << "rotated shape" << std::endl;
                for (auto& point : ts) {
                    std::cout << point.toString() << " ";
                }
                std::cout << "\n";
                auto ms = geometry::calculations::transformPolygon(ts, 0, ip.isMirrored, false, false);
                std::cout << "mirrored shape" << std::endl;
                for (auto& point : ms) {
                    std::cout << point.toString() << " ";
                }
                std::cout << "\n";
                auto ds = ms;
                for (auto& point : ds) {
                    point.x += ip.x;
                    point.y += ip.y;
                }
                std::cout << "translated shape" << std::endl;
                for (auto& point : ds) {
                    std::cout << point.toString() << " ";
                }
                std::cout << "\n";
            }
            
        }

    }

    return 0;
}


