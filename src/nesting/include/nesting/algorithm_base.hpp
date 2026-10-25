#pragma once

#include <vector>
#include <unordered_map>

#include <clipper2/clipper.h>

#include "pattern/pattern_piece.hpp"
#include "pattern/shapes.hpp"
#include "nesting/nested_item.hpp"
#include "nesting/nesting_result.hpp"
#include "geometry/nfp_key.hpp"

namespace pmg::nesting {

class AlgorithmBase {
protected:
    pattern::Fabric m_fabric;
    std::vector<nesting::NestedItem> m_originalItems;
    std::unordered_map<int64_t, Clipper2Lib::Paths64> m_ifpCache; 
    std::unordered_map<geometry::NFPKey, Clipper2Lib::Paths64> m_nfpCache; 

    virtual NestingResult runAlgorithm() = 0;

    NestingResult runSingleBottomLeft(std::vector<nesting::NestedItem>& items);
public:
    AlgorithmBase(pattern::Fabric fabric);
    virtual ~AlgorithmBase() = default;

    const std::unordered_map<int64_t, Clipper2Lib::Paths64>& getIFPCache() const { return m_ifpCache; };
    const std::unordered_map<geometry::NFPKey, Clipper2Lib::Paths64>& getNFPCache() const { return m_nfpCache; };

    NestingResult execute(const std::vector<pattern::PatternPiece>& patternPieces);
};

} // namespace pmg::nesting