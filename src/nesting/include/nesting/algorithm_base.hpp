#pragma once

#include <vector>

#include "pattern/pattern_piece.hpp"
#include "pattern/shapes.hpp"
#include "nesting/nested_item.hpp"

namespace pmg::nesting {

class AlgorithmBase {
protected:
    pattern::Fabric m_fabric;
    std::vector<nesting::NestedItem> m_originalItems;

    std::vector<nesting::NestedItem> prepareItems(const std::vector<pattern::PatternPiece>& patternPieces);

    virtual bool runAlgorithm() = 0;

    bool runSingleBottomLeft(std::vector<nesting::NestedItem>& items);

    bool evaluate(const std::vector<nesting::NestedItem>& items);
public:
    AlgorithmBase(pattern::Fabric fabric);
    virtual ~AlgorithmBase() = default;

    bool execute(const std::vector<pattern::PatternPiece>& patternPieces);
};

} // namespace pmg::nesting