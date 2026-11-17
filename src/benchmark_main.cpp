#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <iostream>
#include <limits>
#include <random>
#include <span>
#include <stdexcept>
#include <vector>

#include "utils/timer.hpp"
#include "geometry/geometry_types.hpp"
#include "pattern/pattern_piece.hpp"
#include "pattern/shapes.hpp"
#include "nesting/nested_item.hpp"
#include "nesting/basic_algorithm.hpp"
#include "nesting/advanced_algorithm.hpp"
#include "nesting/nesting_result.hpp"
#include "sample_input.hpp"

namespace pmg::bench {

template <typename T>
inline void doNotOptimize(T const& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

struct MetricStats {
    double sumNs = 0.0;
    double minNs = std::numeric_limits<double>::max();

    void record(double ns) {
        sumNs += ns;
        minNs = std::min(minNs, ns);
    }

    [[nodiscard]] double avgUs(std::size_t iterations) const { return (sumNs / static_cast<double>(iterations)) / 1000.0; }
    [[nodiscard]] double minUs() const { return minNs / 1000.0; }
};

struct BenchmarkResult {
    std::string engineName;
    std::size_t pieceCount;
    std::size_t twoWayCount;
    MetricStats construct;
    MetricStats nestingPreparation;
    MetricStats nesting;
    MetricStats calculationTotal;
};

BenchmarkResult runBenchmark(
    std::string_view engineName,
    nesting::AlgorithmBase& engine,
    std::span<int64_t> ids,
    std::span<std::vector<geometry::Point64>> shapes,
    std::span<std::vector<pattern::VariantConstraint>> constraints,
    std::size_t twoWayCount,
    int seamAllowance, 
    int cuttingAllowance,
    int grainline, 
    std::size_t iterations,
    std::uint32_t seed = 124125) {

    const std::size_t patternSize = ids.size();
    if (shapes.size() != patternSize || constraints.size() != patternSize || twoWayCount > patternSize) {
        throw std::invalid_argument("Mismatched span sizes or twoWayCount > patternSize");
    }

    // Prepare flipped vector
    std::vector<std::uint8_t> flipped(patternSize, 0);
    std::ranges::fill(flipped.begin(), flipped.begin() + twoWayCount, 1);

    std::mt19937 rng(seed);
    std::ranges::shuffle(flipped, rng);

    for (std::size_t i = 0; i < patternSize; ++i) {
        if (flipped[i]) {
            for (auto& c : constraints[i]) {
                c.twoWay = true;
            }
        }
    }

    // Build input vector lambda
    auto buildPatternPieces = [&]() {
        std::vector<pmg::pattern::PatternPiece> pieces;
        pieces.reserve(patternSize);
        for (std::size_t i = 0; i < patternSize; ++i) {
            pieces.emplace_back(ids[i], shapes[i], constraints[i], seamAllowance, cuttingAllowance, grainline, true);
        }
        return pieces;
    };

    // Test run
    {
        auto warmupPieces = buildPatternPieces();
        doNotOptimize(warmupPieces);
        pmg::timing::PhaseTimings dummy{};
        doNotOptimize(engine.execute(warmupPieces, &dummy));
    }

    BenchmarkResult stats{.engineName = std::string(engineName), .pieceCount = patternSize, .twoWayCount = twoWayCount};

    for (std::size_t iter = 0; iter < iterations; ++iter) {
        auto start = std::chrono::steady_clock::now();
        std::vector<pmg::pattern::PatternPiece> pieces = buildPatternPieces();
        doNotOptimize(pieces);
        auto end = std::chrono::steady_clock::now();

        stats.construct.record(std::chrono::duration<double, std::nano>(end - start).count());

        timing::PhaseTimings timings{};
        pmg::nesting::NestingResult result = engine.execute(pieces, &timings);
        doNotOptimize(result);

        stats.nestingPreparation.record(timings.preparationNs);
        stats.nesting.record(timings.calculationNs);
        stats.calculationTotal.record(timings.totalNs);
    }

    return stats;
}

std::vector<int64_t> getBenchIds() {
    return {
        12428, 12421, 12430, 12429, 12434, 12423, 12422, 12425, 12426, 12431, 12424, 12433, 12427,
        12521, 12523, 12524, 12522, 12526, 12527, 12525,
        12436, 12435
    };
}

std::vector<std::vector<geometry::Point64>> getBenchShapes() {
    return {
        {
            {1221, 1334},
            {1193, 1319},
            {1138, 1262},
            {1129, 1169},
            {1133, 1050},
            {1148, 923},
            {1183, 741},
            {1338, 751},
            {1334, 966},
            {1358, 1181},
            {1338, 1196},
            {1326, 1250},
            {1297, 1306},
            {1263, 1334},
            {1221, 1334}
        },
        {
            {872, 656},
            {885, 597},
            {883, 538},
            {908, 408},
            {920, 297},
            {915, 50},
            {1126, 58},
            {1120, 495},
            {1107, 614},
            {1089, 668},
            {1061, 633},
            {989, 652},
            {973, 698},
            {872, 656}
        },
        {
            {293, 749},
            {255, 752},
            {244, 711},
            {176, 681},
            {135, 719},
            {127, 605},
            {130, 541},
            {175, 135},
            {297, 144},
            {269, 435},
            {269, 563},
            {293, 749}
        },
        {
            {122, 1411},
            {108, 1375},
            {69, 1342},
            {32, 1336},
            {55, 1147},
            {57, 1017},
            {54, 906},
            {143, 894},
            {158, 1079},
            {161, 1185},
            {150, 1314},
            {122, 1411}
        },
        {
            {454, 1059},
            {305, 1026},
            {328, 882},
            {338, 872},
            {472, 889},
            {478, 903},
            {454, 1059}
        },
        {
            {495, 712},
            {447, 712},
            {425, 725},
            {302, 695},
            {324, 583},
            {367, 500},
            {407, 354},
            {407, 74},
            {528, 85},
            {505, 377},
            {507, 560},
            {495, 712}
        },
        {
            {897, 1203},
            {880, 1163},
            {848, 1147},
            {868, 984},
            {863, 832},
            {870, 746},
            {970, 756},
            {954, 898},
            {937, 981},
            {936, 1092},
            {923, 1152},
            {897, 1203}
        },
        {
            {254, 1119},
            {254, 1086},
            {304, 1062},
            {369, 1088},
            {437, 1090},
            {443, 1143},
            {362, 1143},
            {261, 1129},
            {254, 1119}
        },
        {
            {276, 811},
            {286, 776},
            {475, 785},
            {454, 834},
            {381, 830},
            {323, 846},
            {276, 811}
        },
        {
            {445, 1231},
            {430, 1211},
            {402, 1199},
            {369, 1193},
            {370, 1154},
            {402, 1158},
            {449, 1175},
            {481, 1219},
            {445, 1231}
        },
        {
            {898, 1380},
            {650, 1341},
            {536, 1339},
            {462, 1327},
            {402, 1298},
            {467, 1280},
            {491, 1242},
            {658, 1248},
            {780, 1243},
            {902, 1248},
            {898, 1380}
        },
        {
            {206, 1180},
            {206, 1138},
            {353, 1143},
            {353, 1185},
            {206, 1180}
        },
        {
            {246, 839},
            {237, 858},
            {151, 865},
            {106, 859},
            {107, 840},
            {152, 845},
            {246, 839}
        },
        {
            {1409, 1226},
            {1417, 610},
            {1443, 50},
            {1573, 57},
            {1561, 614},
            {1562, 966},
            {1527, 1000},
            {1517, 1065},
            {1549, 1068},
            {1538, 1227},
            {1409, 1226}
        },
        {
            {223, 1501},
            {232, 1349},
            {779, 1377},
            {1373, 1427},
            {1372, 1479},
            {1210, 1486},
            {1150, 1496},
            {1106, 1539},
            {1093, 1598},
            {914, 1551},
            {223, 1501}
        },
        {
            {547, 1171},
            {477, 1159},
            {476, 1008},
            {517, 526},
            {536, 53},
            {682, 58},
            {652, 1042},
            {631, 1187},
            {547, 1171}
        },
        {
            {811, 1199},
            {674, 1193},
            {677, 913},
            {723, 56},
            {847, 61},
            {822, 746},
            {824, 1093},
            {811, 1199}
        },
        {
            {77, 826},
            {67, 789},
            {133, 770},
            {213, 770},
            {263, 775},
            {253, 815},
            {198, 810},
            {135, 810},
            {77, 826}
        },
        {
            {284, 1039},
            {297, 875},
            {246, 871},
            {232, 1034},
            {284, 1039}
        },
        {
            {218, 1081},
            {181, 1095},
            {168, 1017},
            {179, 889},
            {219, 896},
            {211, 1010},
            {218, 1081}
        },
        {
            {1130, 655},
            {1157, 537},
            {1218, 434},
            {1255, 290},
            {1252, 196},
            {1260, 39},
            {1405, 51},
            {1351, 650},
            {1299, 645},
            {1258, 646},
            {1222, 659},
            {1200, 686},
            {1130, 655}
        },
        {
            {1036, 1366},
            {966, 1337},
            {979, 1277},
            {979, 1224},
            {1007, 1059},
            {1006, 748},
            {1097, 749},
            {1090, 1133},
            {1063, 1280},
            {1036, 1366}
        }
    };
}

std::vector<std::vector<pattern::VariantConstraint>> getBenchConstraints() {
    return {
        {
            { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 0, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 1, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 1, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 6, .isMirrored = true, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 6, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 0, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 8, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 8, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 3, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 3, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 6, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 6, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 360-5, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 360-5, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 180+17, .isMirrored = true, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 180+17, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 0, .isMirrored = true, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 90+4, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 90+4, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 1, .isMirrored = false, .requiredQuantity = 2, .twoWay = false },
            { .rotationAngle = 1, .isMirrored = true, .requiredQuantity = 0, .twoWay = false }
        },
        {
            { .rotationAngle = 180, .isMirrored = false, .requiredQuantity = 2, .twoWay = false },
            { .rotationAngle = 180, .isMirrored = true, .requiredQuantity = 2, .twoWay = false }
        },
        {
            { .rotationAngle = 2, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 2, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 270+4, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 270+4, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 2, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 2, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 2, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 2, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 6, .isMirrored = false, .requiredQuantity = 2, .twoWay = false },
            { .rotationAngle = 6, .isMirrored = true, .requiredQuantity = 2, .twoWay = false }
        },
        {
            { .rotationAngle = 5, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 5, .isMirrored = true, .requiredQuantity = 0, .twoWay = false }
        },
        {
            { .rotationAngle = 270+3, .isMirrored = false, .requiredQuantity = 2, .twoWay = false },
            { .rotationAngle = 270+3, .isMirrored = true, .requiredQuantity = 2, .twoWay = false }
        },
        {
            { .rotationAngle = 5, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 5, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
        },
        {
            { .rotationAngle = 1, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
            { .rotationAngle = 1, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
        }
    };

}

} // namespace pmg::bench

int main() {
    constexpr std::size_t ITERATIONS = 2;

    // Prepare input data
    std::vector<int64_t> ids = pmg::bench::getBenchIds();
    std::vector<std::vector<pmg::geometry::Point64>> shapes = pmg::bench::getBenchShapes();
    std::vector<std::vector<pmg::pattern::VariantConstraint>> constraints = pmg::bench::getBenchConstraints();

    std::span<int64_t> fullIds{ids};
    std::span<std::vector<pmg::geometry::Point64>> fullShapes{shapes};
    std::span<std::vector<pmg::pattern::VariantConstraint>> fullConstraints{constraints};

    constexpr int64_t width = 1500;
    constexpr int64_t height = 4500;
    const std::vector<pmg::geometry::Point64> boundary = {
        {1005, 4575}, 
        { 586, 4184}, 
        { 166, 4294}, 
        {  54,   75}, 
        {1430,  229}, 
        {1654, 2873}
    };
    pmg::pattern::Fabric regularFabric(width, height, true);
    pmg::pattern::Fabric irregularFabric(boundary, 0, true);

    // Construct algorithms and fabrics combinations
    struct AlgVariant {
        std::string name;
        std::unique_ptr<pmg::nesting::AlgorithmBase> instance;
    };

    std::vector<AlgVariant> engines;
    engines.emplace_back("QuickAlgorithm_RegularFabric", std::make_unique<pmg::nesting::BasicAlgorithm>(regularFabric));
    engines.emplace_back("QuickAlgorithm_IrregularFabric", std::make_unique<pmg::nesting::BasicAlgorithm>(irregularFabric));
    engines.emplace_back("FullSearchAlgorithm_RegularFabric", std::make_unique<pmg::nesting::AdvancedAlgorithm>(regularFabric));
    engines.emplace_back("FullSearchAlgorithm_IrregularFabric", std::make_unique<pmg::nesting::AdvancedAlgorithm>(irregularFabric));

    // const std::vector<std::size_t> testSizes = {2, 4, 8, 16};
    // const std::vector<double> twoWayRatios = {0.0, 0.25, 0.5, 1.0};
    const std::vector<std::size_t> testSizes {2, 4};
    const std::vector twoWayRatios {0.0, 1.0};
    constexpr int grainline = 0;
    constexpr int seamAllowance = 0;
    constexpr int cuttingAllowance = 0;

    std::cout << "Engine,Size,TwoWay,BuildAvg(us),BuildMin(us),PrepAvg(us),PrepMin(us),CalcAvg(us),CalcMin(us),TotalAvg(us),TotalMin(us)\n";

    for (const auto& engine : engines) {
        for (std::size_t ts : testSizes) {
            for (double ratio : twoWayRatios) {
                auto flags = static_cast<std::size_t>(static_cast<double>(ts) * ratio);
                auto r = pmg::bench::runBenchmark(
                    engine.name,
                    *engine.instance,
                    fullIds.first(ts),
                    fullShapes.first(ts),
                    fullConstraints.first(ts),
                    flags,
                    seamAllowance,
                    cuttingAllowance,
                    grainline,
                    ITERATIONS
                );

                std::cout << std::format("{},{},{},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f}\n",
                    r.engineName, r.pieceCount, r.twoWayCount,
                    r.construct.avgUs(ITERATIONS), r.construct.minUs(),
                    r.nestingPreparation.avgUs(ITERATIONS), r.nestingPreparation.minUs(),
                    r.nesting.avgUs(ITERATIONS), r.nesting.minUs(),
                    r.calculationTotal.avgUs(ITERATIONS), r.calculationTotal.minUs());
            }
        }
    }

    return 0;
}