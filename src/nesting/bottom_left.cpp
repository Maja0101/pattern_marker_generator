#include <spdlog/spdlog.h>

#include "nesting/bottom_left.hpp"
#include "geometry/calculations.hpp"
#include "utils/config.hpp"
#include "utils/timer.hpp"

namespace pmg::nesting {
BLStateView BottomLeft::makeCurrStateView() const
{
    return BLStateView{ m_fabric, m_placed_pieces_polygons, m_placed_pieces_boxes };
}

bool BottomLeft::findPosition(NestedItem &item)
{
    timing::Timer timer;

    // Calculate width and height of item's bounding box and determine the starting position
    Clipper2Lib::Rect64 fabricBounds = m_fabric.getBoundingBox();
    Clipper2Lib::Rect64 item_bounding_box = item.getVariant()->getBoundingBox();

    int64_t item_width = item_bounding_box.Width();
    int64_t item_height = item_bounding_box.Height();

    int64_t current_x = fabricBounds.left;
    int64_t current_y = fabricBounds.bottom - item_height; // In Clipper2 bottom = maxY, top = minY

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("Item w={}, h={}", item_width, item_height));
        SPDLOG_INFO(std::format("Curr x={}, y={}", current_x, current_y));
    }

    // Verify starting position and move right if blocked
    while (!geometry::calculations::canPlaceAt(makeCurrStateView(), item, current_x, current_y)) {
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO("Starting point not valid, moving to the right");
        }
        current_x += m_step_size;
        if (current_x + item_width > fabricBounds.right) {
            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO(std::format("Starting point {} is out of bounds, no possible position", current_x));
            }
            return false;
        }
    }

    // Down-Left loop
    bool wasMoved = true;
    while (wasMoved) {
        wasMoved = false;

        // Slide down 
        while (geometry::calculations::canPlaceAt(makeCurrStateView(), item, current_x, current_y - m_step_size)) {
            current_y -= m_step_size;
            wasMoved = true;
            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO(std::format("Slide down, curr x={}, y={}", current_x, current_y));
            }
        }

        // Slide left
        while (geometry::calculations::canPlaceAt(makeCurrStateView(), item, current_x - m_step_size, current_y)) {
            current_x -= m_step_size;
            wasMoved = true;
            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO(std::format("Slide left, curr x={}, y={}", current_x, current_y));
            }
        }
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("curr x={}, y={}", current_x, current_y));
    }

    // Lock item into place
    m_placed_pieces_polygons.push_back(item.getItemPathAtPosition(current_x, current_y));
    m_placed_pieces_boxes.push_back(item.getItemBoundingBoxAtPosition(current_x, current_y));
    item.lockInPosition(current_x, current_y);

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO("Item locked");
        SPDLOG_INFO(item.toString());
    }

    return true;
}

BottomLeft::BottomLeft(pattern::Fabric fabric, int64_t step_size)
    : m_fabric(std::move(fabric)), m_step_size(step_size) {}

bool BottomLeft::performNesting(std::vector<NestedItem> &items)
{
    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO("Starting nesting");
    }

    timing::Timer timer;

    int i = 0;
    for (auto& item : items) {
        ++i;
        if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO(std::format("Nesting item no. {}", i));
        }
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("Nesting item no. {}", i));
            SPDLOG_INFO(std::format("Nesting for {}", item.toString()));
        }

        if (!findPosition(item)) {
            if constexpr (config::DEBUG_MODE) {
                SPDLOG_INFO("Could not find position");
            }
            return false;
        }
    }

    return true;
}

} // namespace pmg::nesting
