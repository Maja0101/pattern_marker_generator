#include "nesting/algorithm_base.hpp"
#include "nesting/bottom_left.hpp"

namespace pmg::nesting {

std::vector<nesting::NestedItem> AlgorithmBase::prepareItems(const std::vector<pattern::PatternPiece> &patternPieces)
{
    std::vector<nesting::NestedItem> items;

    for (const auto& piece : patternPieces) {
        for (const auto& variant : piece.getVariants()) {
            for (int i = 0; i < variant.getRequiredQuantity(); ++i) {
                items.emplace_back(&variant);
            }
        }
    }
    
    return items;
}

bool AlgorithmBase::runSingleBottomLeft(std::vector<nesting::NestedItem> &items)
{
    // TODO should return the layout and the statistics data

    BottomLeft bottomLeft(m_fabric);

    return bottomLeft.performNesting(items);
}

bool AlgorithmBase::evaluate(const std::vector<nesting::NestedItem> &items)
{
    // TODO evaluation function for layout 

    return false;
}

AlgorithmBase::AlgorithmBase(pattern::Fabric fabric)
    : m_fabric(std::move(fabric)) {}

bool AlgorithmBase::execute(const std::vector<pattern::PatternPiece> &patternPieces)
{
    // TODO should return the layout and the statistics data

    m_originalItems = prepareItems(patternPieces);

    runAlgorithm();

    return false;
}

} // namespace pmg::nesting