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

struct NestingStats {
    double totalFabricArea;
    double totalFabricLength;

    int usableScrapCount;
    double usableScrapsArea;
    double usableScrapsPercentage;

    double remainingFabricLength;
    double remainingFabricArea;
    double remainingFabricPercentage;

    double wasteArea;
    double wastePercentage;
};

struct NestingResult {
    bool wasSuccessful;
    double minLength;
    NestingStats stats;
    std::vector<ItemPlacement> positions;
};

struct NestingScore {
    NestingResult result;
    double score{0.0};
};

} // namespace pmg::nesting