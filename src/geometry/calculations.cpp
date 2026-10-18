#include <spdlog/spdlog.h>

#include "nesting/bottom_left.hpp"
#include "utils/config.hpp"
#include "utils/timer.hpp"
#include "geometry/calculations.hpp"

namespace pmg::geometry::calculations {
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
                SPDLOG_WARN(std::format("Potential obstacle found", x, y));
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

bool canPlaceAt(const nesting::BLStateView &view, const nesting::NestedItem &item, int64_t x, int64_t y)
{
    if (!isWithinFabric(view, item, x, y)) {
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO("Item not wihin fabric");
        }
        return false;
    }

    if (isCollision(view, item, x, y)) {
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO("Item has collision");
        }
        return false;
    }

    return true;
}

std::vector<nesting::NestedItem> prepareItems(const std::vector<pattern::PatternPiece> &patternPieces)
{
    timing::Timer timer;

    std::vector<nesting::NestedItem> items;

    for (const auto& piece : patternPieces) {
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(piece.toString());
        }
        for (const auto& variant : piece.getVariants()) {
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

bool evaluate(const std::vector<nesting::NestedItem> &items)
{
    // TODO evaluation function for layout 
    
    return false;
}

bool isAARectangle(const Clipper2Lib::Path64 &path, const Clipper2Lib::Rect64& boundingBox)
{
    if (path.size() != 4) {
        return false;
    }

    for (const auto& point : path) {
        bool isCorner = 
            (point.x == boundingBox.left || point.x == boundingBox.right) &&
            (point.y == boundingBox.bottom || point.y == boundingBox.top);
        if (!isCorner) {
            return false;
        }
    }

    return true;
}

Clipper2Lib::Path64 invertPath(const Clipper2Lib::Path64 &path)
{
    Clipper2Lib::Path64 inverted;
    inverted.reserve(path.size());
    for (const auto& point : path) {
        inverted.push_back(Clipper2Lib::Point64(-point.x, -point.y));
    }
    return inverted;
}

} // namespace pmg::geometry::calculations