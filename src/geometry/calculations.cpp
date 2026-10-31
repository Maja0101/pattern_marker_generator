#include <unordered_map>

#include <spdlog/spdlog.h>

#include "nesting/bottom_left.hpp"
#include "utils/config.hpp"
#include "utils/timer.hpp"
#include "geometry/calculations.hpp"

namespace pmg::geometry::calculations::utils {
bool aabbCollision(const Clipper2Lib::Rect64 &a, const Clipper2Lib::Rect64 &b)
{
    return (a.left <= b.right && a.right >= b.left) && // (aMinX <= bMaxX && aMaxX >= bMinX) &&
            (a.top <= b.bottom && a.bottom >= b.top); // (aMinY <= bMaxY && aMaxY >= bMinY)
}

bool isCollision(const nesting::BLStateView &view, const nesting::NestedItem &item, int64_t x, int64_t y)
{
    if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("Checking for collision at x={}, y={}", x, y));
    }
    
    // Bounding boxes of item at potential position
    Clipper2Lib::Rect64 itemBoundsAt = item.getItemBoundingBoxAtPosition(x, y);

    // Fast check using bounding boxes and store potential collision shapes
    Clipper2Lib::Paths64 potential_obstacles;

    for (size_t i = 0; i < view.placed_pieces_boxes.size(); ++i) {
        if (aabbCollision(itemBoundsAt, view.placed_pieces_boxes[i])) {
            potential_obstacles.push_back(view.placed_pieces_polygons[i]);
            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO(std::format("Potential obstacle found", x, y));
            }   
        }
    }

    // No collision if there are no overlapping bounding boxes
    if (potential_obstacles.empty()) {
        return false;
    }

    // Item path at potential position
    Clipper2Lib::Path64 itemPathAt = item.getItemPathAtPosition(x, y);

    // Detailed intersection check
    Clipper2Lib::Paths64 intersection = Clipper2Lib::Intersect(
        Clipper2Lib::Paths64{ itemPathAt }, 
        potential_obstacles, 
        Clipper2Lib::FillRule::NonZero
    );

    return !intersection.empty();
}

bool isWithinFabric(const nesting::BLStateView &view, const nesting::NestedItem &item, int64_t x, int64_t y)
{
    // Bounding boxes of fabric and item at potential position
    Clipper2Lib::Rect64 fabricBounds = view.fabric.getBoundingBox();
    Clipper2Lib::Rect64 itemBoundsAt = item.getItemBoundingBoxAtPosition(x, y);

    // Fast check using bounding box
    if (itemBoundsAt.left < fabricBounds.left || itemBoundsAt.right > fabricBounds.right ||
        itemBoundsAt.bottom > fabricBounds.bottom || itemBoundsAt.top < fabricBounds.top) { // bottom is maxY, top is minY
            return false;
    }

    // Item path at potential position
    Clipper2Lib::Path64 itemPathAt = item.getItemPathAtPosition(x, y);

    // Precise check using difference between two paths
    Clipper2Lib::Paths64 overflow = Clipper2Lib::Difference(
        Clipper2Lib::Paths64{ itemPathAt },
        Clipper2Lib::Paths64{ view.fabric.getPath() },
        Clipper2Lib::FillRule::NonZero
    );

    return overflow.empty();
}

int64_t crossProduct(const Point64 &a, const Point64 &b, const Point64 &c)
{
    return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

} // namespace pmg::geometry::calculations::utils

namespace pmg::geometry::calculations {
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

bool canPlaceAt(const nesting::BLStateView &view, const nesting::NestedItem &item, int64_t x, int64_t y)
{
    if (!utils::isWithinFabric(view, item, x, y)) {
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO("Item not wihin fabric");
        }
        return false;
    }

    if (utils::isCollision(view, item, x, y)) {
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO("Item has collision");
        }
        return false;
    }

    return true;
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

nesting::NestingStats evaluate(const Clipper2Lib::Paths64 &allPlacedItems, const pattern::Fabric &fabric, int64_t maxRightPoint)
{
    nesting::NestingStats result;

    constexpr double M_SCALE = 1000000.0; // converstion from internal units to m
    constexpr double MM_SCALE = 1000.0; // converstion from internal units to mm
    constexpr double minWidth = 0.15 * M_SCALE; // set min scrap width to 15 cm
    constexpr double minScrapArea = 0.04; // set min scrap area to 0.04 sm

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
    int64_t padding = std::max(fabric.getBoundingBox().Width(), fabric.getBoundingBox().Height()) + M_SCALE; // max size + 1m 

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
    
    // Clean up scrap
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

    result.usableScrapCount = 0;
    result.usableScrapsArea = 0.0;
    for (const auto& scrap : cleanedScraps) {
        double area = std::abs(Clipper2Lib::Area(scrap)) / (M_SCALE * M_SCALE);
        if (area >= minScrapArea) {
            ++result.usableScrapCount;
            result.usableScrapsArea += area;
        }
    }

    // Calculate remaining fabric area
    result.remainingFabricArea = 0.0;
    for (const auto& fabricPart : rawRemainingFabric) {
        double area = std::abs(Clipper2Lib::Area(fabricPart)) / (M_SCALE * M_SCALE);
        if (area > 0.0) {
            result.remainingFabricArea += area;
        }
    }
    result.remainingFabricArea = std::floor(result.remainingFabricArea * MM_SCALE) / MM_SCALE;

    // Calculate remaining fabric length
    result.remainingFabricLength = std::floor((static_cast<double>(fabric.getBoundingBox().right - maxRightPoint)) / MM_SCALE) / MM_SCALE;

    //Calculate waste
    double fabricArea = std::ceil((std::abs(Clipper2Lib::Area(fabricAsPaths)) / (M_SCALE * M_SCALE)) * MM_SCALE) / MM_SCALE;
    double usedArea = std::ceil((std::abs(Clipper2Lib::Area(allItemsCombined)) / (M_SCALE * M_SCALE)) * MM_SCALE) / MM_SCALE;

    if constexpr (config::DEBUG_MODE) {
        SPDLOG_WARN(std::format("fabricArea {} usedArea {} remainingFabricArea {} usableScrapsArea {}", fabricArea, usedArea, result.remainingFabricArea, result.usableScrapsArea));
    }

    result.wasteArea = fabricArea - usedArea - result.remainingFabricArea - result.usableScrapsArea;

    // Calculate percentages
    double usedAreaPercentage = std::round(usedArea / fabricArea * MM_SCALE) / MM_SCALE;
    result.remainingFabricPercentage = std::round(result.remainingFabricArea / fabricArea * MM_SCALE) / MM_SCALE;
    result.usableScrapsPercentage = std::round(result.usableScrapsArea / fabricArea * MM_SCALE) / MM_SCALE;
    result.wastePercentage = 1.0 - usedAreaPercentage - result.remainingFabricPercentage - result.usableScrapsArea;

    return result;
}

} // namespace pmg::geometry::calculations