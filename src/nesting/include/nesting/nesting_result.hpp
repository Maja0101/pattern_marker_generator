#pragma once

#include <cstdint>
#include <vector>

namespace pmg::nesting {

struct ItemPlacement {
    int64_t id;
    int rotationAngle;
    bool isMirrored;
    bool isFlipped180;
    int64_t x, y;
};

struct NestingResult {
    bool wasSuccessful;

    int64_t minLength;

    double executionTime;
    double remainingFabric;
    int usableScraps;
    double waste;

    std::vector<ItemPlacement> positions;
};

} // namespace pmg::nesting