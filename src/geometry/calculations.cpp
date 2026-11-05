#include <unordered_map>

#include <spdlog/spdlog.h>

#include "nesting/bottom_left.hpp"
#include "utils/config.hpp"
#include "utils/timer.hpp"
#include "geometry/calculations.hpp"
#include "geometry/ifp_pocket_calculations.hpp"

namespace pmg::geometry::calculations::utils {

int64_t crossProduct(const Point64 &a, const Point64 &b, const Point64 &c)
{
    return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

} // namespace pmg::geometry::calculations::utils

namespace pmg::geometry::calculations {

bool isCyclicShift(const std::vector<Point64> &polygonA, const std::vector<Point64> &polygonB)
{
    // Check whether the second polygon is the same as the first polygon
    // even if the starting point does not match

    size_t n = polygonA.size();
    if (n != polygonB.size()) {
        return false;
    }

    for (size_t i = 0; i < n; ++i) {
        if (polygonA[i] == polygonB[0]) {
            bool match = true;
            for (size_t j = 1; j < n; ++j) {
                if (polygonA[(i + j) % n] != polygonB[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                return true;
            }
        }
    }

    return false;
}

Clipper2Lib::Point64 toClipperPoint(const Point64 &point)
{
    return Clipper2Lib::Point64{ point.x, point.y };
}

Clipper2Lib::Path64 toClipperPath(const std::vector<Point64> &polygon)
{
    Clipper2Lib::Path64 path;
    path.reserve(polygon.size());
    for (const auto& point : polygon) {
        path.push_back({point.x, point.y});
    }
    return path;
}

std::vector<Point64> fromClipperPath(const Clipper2Lib::Path64 &path)
{
    std::vector<Point64> polygon;
    polygon.reserve(path.size());
    for (const auto& point : path) {
        polygon.push_back({point.x, point.y});
    }
    return polygon;
}

std::vector<Point64> calculatePolygonWithAllowance(const std::vector<Point64>& polygon, int64_t seamAllowance, int64_t cuttingAllowance)
{
    Clipper2Lib::Paths64 offsetPaths = Clipper2Lib::InflatePaths(
        { toClipperPath(polygon) },
        static_cast<double>(seamAllowance + cuttingAllowance),
        Clipper2Lib::JoinType::Miter,
        Clipper2Lib::EndType::Polygon
    );

    return fromClipperPath(offsetPaths[0]);
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
    for (const auto& nfp : originalNfps) {
        for (const auto& path : nfp) {
            for (const auto& ref : path) {
                if (std::abs(point.x - ref.x) <= epsilon) point.x = ref.x;
                if (std::abs(point.y - ref.y) <= epsilon) point.y = ref.y;
            }
        }
    }

    return point;
}

std::vector<nesting::NestedItem> prepareItems(std::vector<pattern::PatternPiece> &patternPieces)
{
    std::vector<nesting::NestedItem> items;

    for (auto& piece : patternPieces) {

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(piece.toString());
        }

        for (auto& variant : piece.getVariants()) {

            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO(variant.toString());
            }

            for (int i = 0; i < variant.getRequiredQuantity(); ++i) {

                if constexpr (config::DEBUG_MODE_DETAILED) {
                    SPDLOG_INFO(std::format("Loop no. {}", i+1));
                }

                items.emplace_back(&variant);
            }
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
            variantMap.emplace(variant.getGlobalId(false), variant);
        }
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("Processed variants: {}", variantMap.size()));
    }

    std::vector<pattern::PieceVariant> allVariants;
    allVariants.reserve(variantMap.size());

    for (auto& [id, variant] : variantMap) {
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

nesting::NestingStats evaluate(const Clipper2Lib::Paths64 &allPlacedItems, const pattern::Fabric &fabric, int64_t maxRightPoint)
{
    nesting::NestingStats result;

    constexpr int SCALE = 1000; // converstion from internal units to mm
    constexpr int PCS_SCALE = 100; // multiplier for calculating percenatges 
    constexpr int minWidth = 150000; // set min scrap width to 15 cm in internal units
    constexpr int64_t minScrapArea = 40000; // set min scrap area to 0.04 sm in smm

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
    int64_t padding = std::max(fabric.getBoundingBox().Width(), fabric.getBoundingBox().Height()) + 1000000; // max size + 1m 

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
    double delta = (minWidth / 2.0);
    Clipper2Lib::Paths64 cleanedScraps = Clipper2Lib::InflatePaths(
        rawScraps,
        -delta,
        Clipper2Lib::JoinType::Miter, 
        Clipper2Lib::EndType::Polygon
    );

    cleanedScraps = Clipper2Lib::InflatePaths(
        cleanedScraps,
        delta,
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
        int64_t area = std::abs(Clipper2Lib::Area(scrap)) / (SCALE * SCALE);
        if (area >= minScrapArea) {
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
        int64_t area = std::abs(Clipper2Lib::Area(fabricPart)) / (SCALE * SCALE);
        if (area > 0) {
            result.remainingFabricArea += area;
        }
    }

    // Calculate remaining fabric length
    result.totalFabricLength = static_cast<double>(fabric.getBoundingBox().right) / SCALE;
    result.remainingFabricLength = static_cast<double>(fabric.getBoundingBox().right - maxRightPoint) / SCALE;

    //Calculate waste
    result.totalFabricArea = std::abs(Clipper2Lib::Area(fabricAsPaths)) / (SCALE * SCALE);

    int64_t usedArea = std::abs(Clipper2Lib::Area(allItemsCombined)) / (SCALE * SCALE);

    result.wasteArea = result.totalFabricArea - usedArea - result.remainingFabricArea - result.usableScrapsArea;
    
    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("fabricArea {} usedArea {} remainingFabricArea {} usableScrapsArea {} wasteArea {}", 
            result.totalFabricArea, usedArea, result.remainingFabricArea, result.usableScrapsArea, result.wasteArea));
    }

    // Calculate percentages
    result.remainingFabricPercentage = result.remainingFabricArea * PCS_SCALE / result.totalFabricArea;
    result.usableScrapsPercentage = result.usableScrapsArea * PCS_SCALE / result.totalFabricArea;
    result.effectiveScrapsPercentage = result.effectiveScrapsArea * PCS_SCALE / result.totalFabricArea;
    result.wastePercentage = result.wasteArea * PCS_SCALE / result.totalFabricArea;

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("usedAreaPercentage {} remainingFabricPercentage {} usableScrapsPercentage {} wastePercentage {}", 
            usedArea * 100 / result.totalFabricArea, result.remainingFabricPercentage, result.usableScrapsPercentage, result.wastePercentage));
    }

    return result;
}

} // namespace pmg::geometry::calculations