#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "pattern/svg_exporter.hpp"
#include "geometry/options_calculations.hpp"
#include "nesting/advanced_algorithm.hpp"

namespace pmg::nesting {

void AdvancedAlgorithm::evaluateDifferentRotations(std::vector<pattern::NestedItem> &items, size_t index, pattern::NestingScore &best)
{
    if (index == items.size()) {
        pattern::NestingResult currentResult = runSingleBottomLeft(items);

        if (double currentScore = currentResult.wasSuccessful ? geometry::calculations::calculateNestingScore(currentResult.stats) : -1.0; currentScore > best.score) {

            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO(std::format("curr best {} score {} result {} {} {}, {} {} {} {} {}, {} {}", best.score, currentScore, 
                    currentResult.stats.remainingFabricLength,
                    currentResult.stats.remainingFabricArea,
                    currentResult.stats.remainingFabricPercentage,
                    currentResult.stats.usableScrapCount,
                    currentResult.stats.usableScrapsArea,
                    currentResult.stats.usableScrapsPercentage,
                    currentResult.stats.effectiveScrapsArea,
                    currentResult.stats.effectiveScrapsPercentage,
                    currentResult.stats.wasteArea,
                    currentResult.stats.wastePercentage
                ));
            }

            best.result = std::move(currentResult);
            best.score = currentScore;

            if constexpr (config::DEVELOPMENT) {
                visualisation::SvgExporter::exportLayout(
                    std::format("debug_output_{}.svg", ++m_svgExport), 
                    m_fabric,
                    m_originalItems,
                    m_originalItems[0].getVariant()->getSeamAllowance(),
                    m_originalItems[0].getVariant()->getCuttingAllowance()
                );
            }
        }

        for (auto& item : items) {
            item.resetPosition();
        }

        return;
    }

    evaluateDifferentRotations(items, index + 1, best);

    if (items[index].getVariant()->getIsTwoWay()) {
        items[index].flip180();
        evaluateDifferentRotations(items, index + 1, best);
        items[index].flip180();
    }
}

void AdvancedAlgorithm::applyMove(const utils::Move &m)
{
    switch (m.type) {
        using enum pmg::nesting::utils::MoveType;
        case Flip:
            m_originalItems[m.i].flip180();
            break;
        case Swap:
            std::swap(m_originalItems[m.i], m_originalItems[m.j]);
            break;
        case Shift:
            if (m.i < m.j) {
                std::rotate(m_originalItems.begin() + m.i, m_originalItems.begin() + m.i + 1, m_originalItems.begin() + m.j + 1);
            } else if (m.i > m.j) {
                std::rotate(m_originalItems.begin() + m.j, m_originalItems.begin() + m.i, m_originalItems.begin() + m.i + 1);
            }
            break;
    }
}

void AdvancedAlgorithm::undoMove(const utils::Move &m)
{
    switch (m.type) {
        using enum pmg::nesting::utils::MoveType;
        case Flip:
            m_originalItems[m.i].flip180();
            break;
        case Swap:
            std::swap(m_originalItems[m.i], m_originalItems[m.j]);
            break;
        case Shift:
            applyMove(utils::Move{Shift, m.j, m.i});
            break;
    }
}

pattern::NestingScore AdvancedAlgorithm::fullSearch()
{
    pattern::NestingScore best;
    best.score = -1.0;

    if constexpr (config::DEVELOPMENT) SPDLOG_INFO("Running Full Search...");
    m_svgExport = 0;

    do {
        evaluateDifferentRotations(m_originalItems, 0, best);
    } while (std::ranges::next_permutation(m_originalItems, [](const pattern::NestedItem& a, const pattern::NestedItem&b) {
        return a.getVariant()->getGlobalId(false) < b.getVariant()->getGlobalId(false);
    }).found);

    if constexpr (config::DEBUG_MODE) {
        SPDLOG_INFO("Last Svg {}", m_svgExport);
    }

    return best;
}

pattern::NestingScore AdvancedAlgorithm::checkOneOption()
{
    pattern::NestingScore best;
    best.score = -1.0;

    if constexpr (config::DEVELOPMENT) SPDLOG_INFO("Running Smart Single Bottom Left...");

    int64_t currIdx = m_originalItems[0].getVariant()->getGlobalId(false);
    size_t currIdxCount = 0;

    for (auto& item : m_originalItems) {
        if (item.getVariant()->getGlobalId(false) != currIdx) {
            currIdx = item.getVariant()->getGlobalId(false);
            currIdxCount = 0;
        }
            
        if (currIdxCount % 2 != 0) {
            item.flip180();
        }

        ++currIdxCount;
    }

    std::ranges::sort(m_originalItems, [](const pattern::NestedItem&a, const pattern::NestedItem& b) {
        auto aBounds = a.getItemBoundingBoxAtPosition();
        auto bBounds = b.getItemBoundingBoxAtPosition();
        return (aBounds.bottom - aBounds.top) > (bBounds.bottom - bBounds.top);
    });

    best.result = runSingleBottomLeft(m_originalItems);

    if constexpr (config::DEVELOPMENT) {
        visualisation::SvgExporter::exportLayout(
            "debug_output.svg", 
            m_fabric,
            m_originalItems,
            m_originalItems[0].getVariant()->getSeamAllowance(),
            m_originalItems[0].getVariant()->getCuttingAllowance()
        );
    }

    return best;
}

pattern::NestingScore AdvancedAlgorithm::evaluate()
{
    constexpr double FAILED_SCORE = -100.0;

    pattern::NestingResult res = runSingleBottomLeft(m_originalItems);

    return pattern::NestingScore{
        .result = res,
        .score = res.wasSuccessful ? geometry::calculations::calculateNestingScore(res.stats) : FAILED_SCORE
    };
}

void AdvancedAlgorithm::resetAllPositions()
{
    for (auto& item : m_originalItems) {
        item.resetPosition();
    }
}

bool AdvancedAlgorithm::checkAllIdentical()
{
    return std::all_of(m_originalItems.begin() + 1, m_originalItems.end(),
            [&](const pattern::NestedItem& item) { return item == m_originalItems[0]; });
}

size_t AdvancedAlgorithm::pickBiasedIndex(size_t n, double exponent)
{
    // exponent > 1.0 favours indexes near 0, exponent == 1.0 is uniform
    double u = m_probDist(m_rng);
    auto idx = static_cast<size_t>(static_cast<double>(n) * std::pow(u, exponent));
    return std::min(idx, n - 1);
}

bool AdvancedAlgorithm::pickValidMove(size_t n, bool identical, utils::Move &outMove, double biasExponent)
{
    if (size_t randIdx = pickBiasedIndex(n, biasExponent); m_originalItems[randIdx].canBeFlipped() && m_probDist(m_rng) < 0.5) {
        outMove = {utils::MoveType::Flip, randIdx, 0};
        return true;
    }

    if (n <= 1 || identical) return false;

    if (m_probDist(m_rng) < 0.5) {
        int guard = 0;
        while (++guard < 50) {
            size_t i = pickBiasedIndex(n, biasExponent);
            size_t j = m_idxDist(m_rng);
            if (i != j && m_originalItems[i] != m_originalItems[j]) {
                outMove = {utils::MoveType::Swap, i, j};
                return true;
            }
        }
    } else {
        int guard = 0;
        while (++guard < 50) {
            size_t i = pickBiasedIndex(n, biasExponent);
            size_t j = pickBiasedIndex(n, biasExponent);
            if (i == j) continue;

            auto [lo, hi] = std::minmax(i, j);
            bool changesState = false;
            for (size_t k = lo; k <= hi; ++k) {
                if (m_originalItems[k] != m_originalItems[i]) {
                    changesState = true;
                    break;
                }
            }
            if (changesState) {
                outMove = {utils::MoveType::Shift, i, j};
                return true;
            }
        }
    }
    return false;
}

double AdvancedAlgorithm::setInitialTemp(const pattern::NestingScore &currentScore, pattern::NestingScore &bestScore, std::vector<pattern::NestedItem> &bestItems, size_t n, bool identical)
{
    double temp = 1.5;
    double totalDrop = 0.0;
    int dropCount = 0;

    for (int s = 0; s < 10; ++s) {
        utils::Move sampleMove{};
        if (!pickValidMove(n, identical, sampleMove, 1.5)) continue;

        applyMove(sampleMove);
        pattern::NestingScore sampleScore = evaluate();

        if (sampleScore.result.wasSuccessful && sampleScore.score > bestScore.score) {
            bestScore = sampleScore;
            
            if constexpr (config::DEVELOPMENT) {
                visualisation::SvgExporter::exportLayout(
                    std::format("debug_output_{}.svg", ++m_svgExport), 
                    m_fabric,
                    m_originalItems,
                    m_originalItems[0].getVariant()->getSeamAllowance(),
                    m_originalItems[0].getVariant()->getCuttingAllowance()
                );
            }

            resetAllPositions();
            bestItems = m_originalItems;
        } else {
            resetAllPositions();
        }
        undoMove(sampleMove);

        double diff = sampleScore.score - currentScore.score;
        if (diff < 0.0 && sampleScore.result.wasSuccessful) {
            totalDrop += std::abs(diff);
            ++dropCount;
        }
    }

    if (dropCount > 0) {
        double avgDrop = totalDrop / dropCount;
        // ~80% initial acceptance for an average worse move (-1 / ln(0.8) ≈ 4.48)
        temp = std::clamp(avgDrop * 4.48, 0.1, 25.0);
    }

    return temp;
}


pattern::NestingScore AdvancedAlgorithm::simulatedAnnealing()
{
    if constexpr (config::DEVELOPMENT) SPDLOG_INFO("Running Simulated Annealing...");
    m_svgExport = 0;

    const size_t n = m_originalItems.size();
    if (n == 0) return {};

    // Initial solution - sort by height
    std::ranges::sort(m_originalItems, [](const pattern::NestedItem& a, const pattern::NestedItem& b) {
        auto aBounds = a.getItemBoundingBoxAtPosition();
        auto bBounds = b.getItemBoundingBoxAtPosition();
        return (aBounds.bottom - aBounds.top) > (bBounds.bottom - bBounds.top);
    });

    // Set parameters depending on the problem size
    const double TEMP_MIN = 0.01;
    const double ALPHA = (n < 15) ? 0.96 : 0.98;
    const int STEPS_PER_TEMP = std::clamp(static_cast<int>(2 * n), 10, 50);
    const int MAX_NO_IMPROVEMENT = 25;
    constexpr double MIN_MEANINGFUL_GAIN = 0.02;

    m_idxDist = std::uniform_int_distribution<size_t>{0, n - 1};

    bool identical = checkAllIdentical();

    pattern::NestingScore currentScore = evaluate();
    pattern::NestingScore bestScore = currentScore;

    if constexpr (config::DEVELOPMENT) {
        visualisation::SvgExporter::exportLayout(
            std::format("debug_output_{}.svg", ++m_svgExport), 
            m_fabric,
            m_originalItems,
            m_originalItems[0].getVariant()->getSeamAllowance(),
            m_originalItems[0].getVariant()->getCuttingAllowance()
        );
    }

    resetAllPositions();
    std::vector<pattern::NestedItem> bestItems = m_originalItems;

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("Initial score {}", bestScore.score));
    }

    // Set starting temperature
    double temp = setInitialTemp(currentScore, bestScore, bestItems, n, identical);

    int noImprovement = 0;
    const double initialTemp = temp;

    while (temp > TEMP_MIN && noImprovement < MAX_NO_IMPROVEMENT) {
        bool improved = false;

        double progress = std::clamp(1.0 - (temp / initialTemp), 0.0, 1.0);
        double currentBias = 1.8 - (0.8 * progress);

        for (int step = 0; step < STEPS_PER_TEMP; ++step) {
            utils::Move move{};
            if (!pickValidMove(n, identical, move, currentBias)) {
                continue;
            }

            applyMove(move);
            pattern::NestingScore candidateScore = evaluate();
            double deltaE = candidateScore.score - currentScore.score;

            // Positive delta meand better result
            if (deltaE > 0.0 || m_probDist(m_rng) < std::exp(deltaE / temp)) {
                currentScore = candidateScore;
                if (currentScore.result.wasSuccessful && currentScore.score > bestScore.score) {
                    if (currentScore.score > bestScore.score + MIN_MEANINGFUL_GAIN) improved = true;

                    bestScore = currentScore;

                    if constexpr (config::DEBUG_MODE_DETAILED) {
                        SPDLOG_INFO(std::format("temp {} step {} delta {}", temp, step, deltaE));
                    }

                    if constexpr (config::DEVELOPMENT) {
                        visualisation::SvgExporter::exportLayout(
                            std::format("debug_output_{}.svg", ++m_svgExport), 
                            m_fabric,
                            m_originalItems,
                            m_originalItems[0].getVariant()->getSeamAllowance(),
                            m_originalItems[0].getVariant()->getCuttingAllowance()
                        );
                    }
                    
                    if constexpr (config::DEBUG_MODE_DETAILED) {
                        SPDLOG_INFO(std::format("Score improved to {} for last svg {}", bestScore.score, m_svgExport));
                    }

                    resetAllPositions();
                    bestItems = m_originalItems;
                } else {
                    resetAllPositions();
                }
                if (move.type == utils::MoveType::Flip && n > 1) {
                    identical = checkAllIdentical();
                }
            } else {
                resetAllPositions();
                undoMove(move);
            }
        }

        noImprovement = improved ? 0 : (noImprovement + 1);
        temp *= ALPHA;
    }

    m_originalItems = std::move(bestItems);

    if constexpr (config::DEBUG_MODE) {
        SPDLOG_INFO("Last Svg {}", m_svgExport);
    }

    return bestScore;
}

pattern::NestingResult AdvancedAlgorithm::runAlgorithm()
{
    std::ranges::sort(m_originalItems, [](const pattern::NestedItem&a, const pattern::NestedItem& b) {
        return a.getVariant()->getGlobalId(false) < b.getVariant()->getGlobalId(false);
    });

    if(geometry::calculations::numberOfOptionsWithinLimit(m_originalItems, 10000)) {
        return fullSearch().result;
    } 
    
    return simulatedAnnealing().result;
}

AdvancedAlgorithm::AdvancedAlgorithm(int64_t width, int64_t length)
    : AlgorithmBase(width, length), m_rng(124125), m_probDist(0.0, 1.0) {}

AdvancedAlgorithm::AdvancedAlgorithm(pattern::Fabric fabric)
    : AlgorithmBase(fabric), m_rng(124125), m_probDist(0.0, 1.0) {}

} // namespace pmg::nesting