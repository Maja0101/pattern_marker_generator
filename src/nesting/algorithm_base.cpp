#include <spdlog/spdlog.h>

#include "utils/config.hpp"
#include "nesting/algorithm_base.hpp"
#include "nesting/bottom_left.hpp"
#include "geometry/calculations.hpp"
#include "geometry/nfp_ifp_calculations.hpp"

namespace pmg::nesting {

bool AlgorithmBase::runSingleBottomLeft(std::vector<nesting::NestedItem> &items)
{
    // TODO should return the layout and the statistics data

    BottomLeft bottomLeft(m_fabric, m_ifpCache, m_nfpCache);

    return bottomLeft.performNesting(items);
}

AlgorithmBase::AlgorithmBase(pattern::Fabric fabric)
    : m_fabric(std::move(fabric)) {}

bool AlgorithmBase::execute(const std::vector<pattern::PatternPiece> &patternPieces)
{
    // TODO should return the layout and the statistics data

    m_originalItems = geometry::calculations::prepareItems(patternPieces);

    std::vector<pmg::pattern::PieceVariant> allPossibleVariants = geometry::calculations::getAllVariants(patternPieces);

    m_ifpCache = geometry::calculations::calculateIFPCache(m_fabric, allPossibleVariants);
    m_nfpCache = geometry::calculations::calculateNFPCache(allPossibleVariants);

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& item : m_originalItems) {
            SPDLOG_INFO(item.toString());
        }
    }

    runAlgorithm();

    return false;
}

} // namespace pmg::nesting