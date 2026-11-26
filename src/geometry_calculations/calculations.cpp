#include <algorithm>
#include <cstddef>
#include <unordered_map>
#include <array>
#include <cmath>
#include <ranges>

#include <spdlog/spdlog.h>

#include "common/helpers.hpp"
#include "utils/config.hpp"
#include "geometry_calculations/calculations.hpp"
#include "geometry_calculations/ifp_pocket_calculations.hpp"
#include "pattern/svg_exporter.hpp"

namespace pmg::geometry::calculations::utils {

std::array<int, 4> calculatePercentages(const std::array<int64_t, 4>& areas, const int64_t totalArea) {
    if (totalArea == 0) {
        return {0, 0, 0, 0};
    }

    std::array<int, 4> results;
    std::array<int64_t, 4> remainders;

    int sum = 0;
    const int PCS_SCALE = 100;
    for (int i = 0; i < 4; ++i) {
        results[i] = static_cast<int>((areas[i] * PCS_SCALE) / totalArea);
        remainders[i] = (areas[i] * PCS_SCALE) % totalArea;
        sum += results[i];
    }

    std::array<int, 4> indexes = {0, 1, 2, 3};

    std::ranges::stable_sort(indexes, [&](int a, int b) {
        return remainders[a] > remainders[b];
    });

    const int LEFT = 100 - sum;
    for (int i = 0; i < LEFT; ++i) {
        results[indexes[i]] += 1;
    }

    return results;
}

} // namespace pmg::geometry::calculations::utils

namespace pmg::geometry::calculations {

Clipper2Lib::Point64 toClipperPoint(const Point64 &point)
{
    return Clipper2Lib::Point64{ point.x, point.y };
}

Clipper2Lib::Paths64 transaltePaths(const Clipper2Lib::Paths64 &paths, const Clipper2Lib::Point64 &point)
{
    Clipper2Lib::Paths64 result = paths;
    for (auto& path : result) {
        for (auto& p : path) {
            p.x += point.x;
            p.y += point.y;
        }
    }
    return result;
}

Point64 rotatePointCcwBy(const Point64 &point, int angle) {
    const int GRAINLINE_CCW = 360 - angle;
    return transformPolygon({ point }, GRAINLINE_CCW, false, false)[0];
}
Point64 rotatePointCwBy(const Point64 &point, int angle)
{
    return transformPolygon({ point }, angle, false, false)[0];
};

Clipper2Lib::Point64 snapToBoundaries(Clipper2Lib::Point64 point, const Clipper2Lib::Paths64 &ifpPaths, const std::vector<Clipper2Lib::Paths64> &originalNfps, int64_t epsilon)
{
    // Snap to IFP 
    for (const auto& path : ifpPaths) {
        for (const auto& ref : path) {
            if (std::abs(point.x - ref.x) <= epsilon) point.x = ref.x;
            if (std::abs(point.y - ref.y) <= epsilon) point.y = ref.y;
        }
    }

    // Snap to original NFP
    auto snap = [&](auto& val, auto target) {
        if (std::abs(val - target) <= epsilon) val = target;
    };

    for (const auto& ref : originalNfps | std::views::join | std::views::join) {
        snap(point.x, ref.x);
        snap(point.y, ref.y);
    }

    return point;
}

std::vector<pattern::NestedItem> prepareItems(std::vector<pattern::PatternPiece> &patternPieces)
{
    size_t total = 0;
    for (auto& piece : patternPieces) {
        for (const auto& variant : piece.getVariants()) {
            total += variant.getRequiredQuantity();
        }
    }

    std::vector<pattern::NestedItem> items;
    items.reserve(total);

    for (auto& piece : patternPieces) {

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(piece.toString());
        }

        for (auto& variant : piece.getVariants()) {

            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO(variant.toString());
            }

            items.insert(items.end(), variant.getRequiredQuantity(), pattern::NestedItem(&variant));
        }
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& item : items) {
            SPDLOG_INFO(item.toString());
        }
    }
    
    return items;
}

std::vector<pattern::PieceVariant> getAllVariants(std::vector<pattern::PatternPiece> &patternPieces)
{
    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("Number of pattern pieces: {}", patternPieces.size()));
    }

    std::unordered_map<int64_t, pattern::PieceVariant> variantMap;

    for (auto& piece : patternPieces) {

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("Pattern pieces no. {}", piece.getId()));
            SPDLOG_INFO(std::format("Number of variants: {}", piece.getVariants().size()));
        }

        for (auto& variant : piece.getVariants()) {
            variantMap.try_emplace(variant.getGlobalId(false), variant);
        }
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("Processed variants: {}", variantMap.size()));
    }

    std::vector<pattern::PieceVariant> allVariants;
    allVariants.reserve(variantMap.size());

    for (const auto& [id, variant] : variantMap) {
        allVariants.push_back(variant);
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("Number of all variants: {}", allVariants.size()));
    }

    return allVariants;
}

double calculateSolidity(const Clipper2Lib::Path64 &scrapPath)
{
    // Solidity is calculated as scrap area divided by scrap's convex hull area
    double scrapArea = std::abs(Clipper2Lib::Area(scrapPath));
    if (scrapArea <= 0) return 0.0;

    Clipper2Lib::Path64 scrapConvexHull = toClipperPath(utils::calculateConvexHull(fromClipperPath(scrapPath)));
    double convexHullArea = std::abs(Clipper2Lib::Area(scrapConvexHull));
    if (convexHullArea <= 0) return 0.0;

    double solidity = scrapArea / convexHullArea;
    return std::min(1.0, std::max(0.0, solidity));
}

pattern::NestingStats evaluate(const Clipper2Lib::Paths64 &allPlacedItems, const pattern::Fabric &fabric, int64_t maxRightPoint)
{
    pattern::NestingStats result;

    constexpr int SCALE = 1000; // converstion from internal units to mm
    constexpr int PCS_SCALE = 100; // multiplier for calculating percenatges 
    constexpr int MIN_WIDTH = 150000; // set min scrap width to 15 cm in internal units
    constexpr int64_t MIN_SCRAP_AREA = 40000; // set min scrap area to 0.04 sm in smm

    // Prepare fabric and placed items for calculation
    Clipper2Lib::Paths64 fabricAsPaths = { fabric.getPath() };
    Clipper2Lib::Paths64 allItemsCombined = Clipper2Lib::Union(allPlacedItems, Clipper2Lib::FillRule::NonZero);

    // Calculate the paths of all parts of unused fabric
    Clipper2Lib::Paths64 rawUnusedFabric = Clipper2Lib::Difference(
        fabricAsPaths,
        allItemsCombined,
        Clipper2Lib::FillRule::NonZero
    );

    // Create bounding box to the right of the max right point
    const int64_t METRE = 1000000;
    int64_t padding = std::max(fabric.getBoundingBox().Width(),
                               fabric.getBoundingBox().Height()) +
                               METRE; // max size + 1m

    Clipper2Lib::Path64 rightBoxPath = {
        {maxRightPoint, fabric.getBoundingBox().top - padding},
        {fabric.getBoundingBox().right, fabric.getBoundingBox().top - padding},
        {fabric.getBoundingBox().right, fabric.getBoundingBox().bottom + padding},
        {maxRightPoint, fabric.getBoundingBox().bottom + padding}
    };
    Clipper2Lib::Paths64 rightBox = { rightBoxPath };
    
    // Get the fabric to the right of the furthest piece
    Clipper2Lib::Paths64 rawRemainingFabric = Clipper2Lib::Intersect(
        rawUnusedFabric,
        rightBox,
        Clipper2Lib::FillRule::NonZero
    );
    
    // Get the scraps - unused fabric to the left of the furthest piece
    Clipper2Lib::Paths64 rawScraps = Clipper2Lib::Difference(
        rawUnusedFabric,
        rightBox,
        Clipper2Lib::FillRule::NonZero
    );

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& path : rawScraps) {
            for (const auto& point : path) {
                SPDLOG_INFO(std::format("raw scrap point ({}, {})", point.x, point.y));
            }
        }
    }
    
    // Clean up scraps
    // Deflating and Inflating scraps' paths discard the tiny and narrow scraps that are not usable
    const double DELTA = (MIN_WIDTH / 2.0);
    Clipper2Lib::Paths64 cleanedScraps = Clipper2Lib::InflatePaths(
        rawScraps,
        -DELTA,
        Clipper2Lib::JoinType::Miter, 
        Clipper2Lib::EndType::Polygon
    );

    cleanedScraps = Clipper2Lib::InflatePaths(
        cleanedScraps,
        DELTA,
        Clipper2Lib::JoinType::Miter, 
        Clipper2Lib::EndType::Polygon
    );

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& path : cleanedScraps) {
            for (const auto& point : path) {
                SPDLOG_INFO(std::format("cleaned scrap point ({}, {})", point.x, point.y));
            }
        }
    }

    // Calculate stats for scraps
    result.usableScrapCount = 0;
    result.usableScrapsArea = 0;
    result.effectiveScrapsArea = 0;
    for (const auto& scrap : cleanedScraps) {
        int64_t area = std::abs(static_cast<int64_t>(Clipper2Lib::Area(scrap))) / (static_cast<long>(SCALE * SCALE));
        if (area >= MIN_SCRAP_AREA) {
            ++result.usableScrapCount;
            result.usableScrapsArea += area;
            double solidity = calculateSolidity(scrap);
            if (solidity == 1.0) {
                result.effectiveScrapsArea += area;
            } else if (solidity > 0.0) {
                result.effectiveScrapsArea += static_cast<int64_t>(static_cast<long double>(area) * solidity);
            }
        }
    }

    // Calculate remaining fabric area
    result.remainingFabricArea = 0;
    for (const auto& fabricPart : rawRemainingFabric) {
        int64_t area = std::abs(static_cast<int64_t>(Clipper2Lib::Area(fabricPart))) / (static_cast<long>(SCALE * SCALE));
        if (area > 0) {
            result.remainingFabricArea += area;
        }
    }

    // Calculate remaining fabric length
    result.totalFabricLength = (fabric.getBoundingBox().right) / SCALE;
    result.remainingFabricLength = (fabric.getBoundingBox().right - maxRightPoint) / SCALE;

    //Calculate waste
    result.totalFabricArea = std::abs(static_cast<int64_t>(Clipper2Lib::Area(fabricAsPaths))) / (static_cast<long>(SCALE * SCALE));

    int64_t usedArea = std::abs(static_cast<int64_t>(Clipper2Lib::Area(allItemsCombined))) / (static_cast<long>(SCALE) * SCALE);

    result.wasteArea = result.totalFabricArea - usedArea - result.remainingFabricArea - result.usableScrapsArea;
    
    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("fabricArea {} usedArea {} remainingFabricArea {} usableScrapsArea {} wasteArea {}", 
            result.totalFabricArea, usedArea, result.remainingFabricArea, result.usableScrapsArea, result.wasteArea));
    }

    // Calculate percentages
    auto [
        usedAreaPercentage, 
        remainingFabricPercentage, 
        usableScrapsPercentage, 
        wastePercentage
    ] = utils::calculatePercentages({
        usedArea,
        result.remainingFabricArea,
        result.usableScrapsArea,
        result.wasteArea
    }, result.totalFabricArea);

    result.remainingFabricPercentage = remainingFabricPercentage;
    result.usableScrapsPercentage = usableScrapsPercentage;
    result.wastePercentage = wastePercentage;

    result.effectiveScrapsPercentage = static_cast<int>((result.effectiveScrapsArea * PCS_SCALE + (result.totalFabricArea / 2)) / result.totalFabricArea);

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("usedAreaPercentage {} remainingFabricPercentage {} usableScrapsPercentage {} effectiveScrapsPercentage {} wastePercentage {}", 
            usedArea * 100 / result.totalFabricArea, result.remainingFabricPercentage, result.usableScrapsPercentage, result.effectiveScrapsPercentage, result.wastePercentage));
    }

    return result;
}

pattern::NestingStats recalculateStats(const pattern::Fabric &fabric, std::vector<pattern::NestedItem> &nestedItems, const std::vector<pattern::ItemPlacement> &newPlacements)
{
    const int SCALE = 1000;
    Clipper2Lib::Paths64 placedItemsPaths;

    size_t n = newPlacements.size();
    int64_t minLen = 0;
    for (size_t i = 0; i < n; ++i) {
        const int64_t placementX = static_cast<int64_t>(std::llround(newPlacements[i].x * static_cast<double>(SCALE)));
        const int64_t placementY = static_cast<int64_t>(std::llround(newPlacements[i].y * static_cast<double>(SCALE)));

        geometry::Point64 unrotated = rotatePointCwBy(
            geometry::Point64{placementX, placementY}, 
            fabric.getGrainline()
        );

        geometry::Point64 fabricRefPoint = fabric.getReferencePoint();
        geometry::Point64 refPoint = nestedItems[i].getVariant()->getReferencePoint();

        Point64 rawPosition{
            (fabricRefPoint.y - refPoint.y) - unrotated.y,
            unrotated.x - (fabricRefPoint.x - refPoint.x)
        };

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("{} {} {}; {} {} {} {} {}", 
                unrotated.toString(),
                fabricRefPoint.toString(),
                refPoint.toString(),
                nestedItems[i].getVariant()->getPieceId(), 
                nestedItems[i].getVariant()->getRotationAngle(), 
                nestedItems[i].getVariant()->getIsMirrored(), 
                nestedItems[i].isFlipped(), rawPosition.toString()
            ));
        }

        placedItemsPaths.push_back(nestedItems[i].getItemPathAtPosition(rawPosition.x, rawPosition.y));
        nestedItems[i].lockInPosition(rawPosition.x, rawPosition.y);
        minLen = std::max(nestedItems[i].getItemBoundingBoxAtPosition().right, minLen);
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("{} {}", newPlacements.size(), nestedItems.size()));
        for (size_t i = 0; i < nestedItems.size(); ++i) {
            SPDLOG_INFO(std::format("ip: {} {} {} {} ({}, {}); ni: {} {} {} {} {}", 
                newPlacements[i].id, newPlacements[i].rotationAngle, newPlacements[i].isMirrored, newPlacements[i].isFlipped180, newPlacements[i].x, newPlacements[i].y,
                nestedItems[i].getVariant()->getPieceId(), nestedItems[i].getVariant()->getRotationAngle(), nestedItems[i].getVariant()->getIsMirrored(), nestedItems[i].isFlipped(), nestedItems[i].getPosition().toString()
            ));
        }
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        visualisation::SvgExporter::exportLayout(
            "recreated_layout.svg", 
            fabric,
            nestedItems,
            nestedItems[0].getVariant()->getSeamAllowance(),
            nestedItems[0].getVariant()->getCuttingAllowance()
        );
    }

    return geometry::calculations::evaluate(placedItemsPaths, fabric, minLen);
}

} // namespace pmg::geometry::calculations