#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "utils/timer.hpp"
#include "geometry/calculations.hpp"
#include "geometry/nfp_ifp_calculations.hpp"
#include "nesting/bottom_left.hpp"
#include "nesting/algorithm_base.hpp"

namespace pmg::nesting {

NestingResult AlgorithmBase::runSingleBottomLeft(std::vector<nesting::NestedItem> &items)
{
    BottomLeft bottomLeft(m_fabric, m_ifpCache, m_nfpCache);

    return bottomLeft.performNesting(items);
}

AlgorithmBase::AlgorithmBase(int64_t width, int64_t length)
    : m_fabric(width, length) {}

AlgorithmBase::AlgorithmBase(pattern::Fabric fabric)
    : m_fabric(std::move(fabric)) {}

NestingResult AlgorithmBase::execute(std::vector<pattern::PatternPiece> &patternPieces)
{
    {
        if constexpr (config::DEVELOPMENT) SPDLOG_INFO("Preparing the geometry...");
        
        timing::Timer timer;

        m_originalItems = geometry::calculations::prepareItems(patternPieces);

        std::vector<pmg::pattern::PieceVariant> allPossibleVariants = geometry::calculations::getAllVariants(patternPieces);

        m_ifpCache = geometry::calculations::calculateIFPCache(m_fabric, allPossibleVariants);
        m_nfpCache = geometry::calculations::calculateNFPCache(allPossibleVariants);

        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("ifp cache: {}, nfp cache: {}", m_ifpCache.size(), m_nfpCache.size()));
            for (const auto& [key, val] : m_ifpCache) {
                SPDLOG_INFO(std::format("ifp cache id {}", key));
            }
            for (const auto& [key, val] : m_nfpCache) {
                SPDLOG_INFO(std::format("nfp cache id {} {}", key.variantIdA, key.variantIdB));
            }
        }

        if constexpr (config::DEBUG_MODE_DETAILED) {
            for (const auto& item : m_originalItems) {
                SPDLOG_INFO(item.toString());
            }
        }
    }

    NestingResult result;
    {
        if constexpr (config::DEVELOPMENT) SPDLOG_INFO("Nesting...");
        timing::Timer timer;
        result = runAlgorithm();
    }


    return result;
}

} // namespace pmg::nesting