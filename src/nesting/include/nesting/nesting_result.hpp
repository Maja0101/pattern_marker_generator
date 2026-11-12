#pragma once

#include <cstdint>
#include <vector>

namespace pmg::nesting {

struct ItemPlacement {
    int64_t id;
    int rotationAngle;
    bool isMirrored;
    bool isFlipped180;
    int64_t x;
    int64_t y;
};

struct NestingStats {
    int64_t totalFabricArea;
    int64_t totalFabricLength;

    int usableScrapCount;
    int64_t usableScrapsArea;
    int usableScrapsPercentage;
    int64_t effectiveScrapsArea;
    int effectiveScrapsPercentage;

    int64_t remainingFabricLength;
    int64_t remainingFabricArea;
    int remainingFabricPercentage;

    int64_t wasteArea;
    int wastePercentage;
};

struct NestingResult {
    bool wasSuccessful;
    int64_t minLength;
    int executionTime;
    NestingStats stats;
    std::vector<ItemPlacement> positions;
};

struct NestingScore {
    NestingResult result;
    double score{0.0};
};

} // namespace pmg::nesting