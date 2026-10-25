#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "utils/timer.hpp"
#include "nesting/algorithm_base.hpp"
#include "nesting/bottom_left.hpp"
#include "geometry/calculations.hpp"
#include "geometry/nfp_ifp_calculations.hpp"

namespace pmg::nesting {

NestingResult AlgorithmBase::runSingleBottomLeft(std::vector<nesting::NestedItem> &items)
{
    // TODO layout and the statistics data

    BottomLeft bottomLeft(m_fabric, m_ifpCache, m_nfpCache);

    return bottomLeft.performNesting(items);
}

AlgorithmBase::AlgorithmBase(pattern::Fabric fabric)
    : m_fabric(std::move(fabric)) {}

NestingResult AlgorithmBase::execute(const std::vector<pattern::PatternPiece> &patternPieces)
{
    // TODO layout and the statistics data

    {
        SPDLOG_INFO("Preparing the geometry...");
        timing::Timer timer;

        m_originalItems = geometry::calculations::prepareItems(patternPieces);

        std::vector<pmg::pattern::PieceVariant> allPossibleVariants = geometry::calculations::getAllVariants(patternPieces);

        m_ifpCache = geometry::calculations::calculateIFPCache(m_fabric, allPossibleVariants);
        m_nfpCache = geometry::calculations::calculateNFPCache(allPossibleVariants);

        if constexpr (config::DEBUG_MODE_DETAILED) {
            for (const auto& item : m_originalItems) {
                SPDLOG_INFO(item.toString());
            }
        }
    }

    NestingResult result;
    {
        SPDLOG_INFO("Nesting...");
        timing::Timer timer;
        result = runAlgorithm();
    }


    return result;
}

} // namespace pmg::nesting