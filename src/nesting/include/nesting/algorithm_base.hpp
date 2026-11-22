#pragma once

#include <vector>
#include <unordered_map>

#include <clipper2/clipper.h>

#include "pattern/pattern_piece.hpp"
#include "pattern/shapes.hpp"
#include "pattern/nested_item.hpp"
#include "pattern/nesting_result.hpp"
#include "geometry/nfp_key.hpp"
#include "utils/phase_timings.hpp"

namespace pmg::nesting {

class AlgorithmBase {
private:
    void reset();
protected:
    pattern::Fabric m_fabric;
    std::vector<pattern::NestedItem> m_originalItems;
    std::unordered_map<int64_t, Clipper2Lib::Paths64> m_ifpCache; 
    std::unordered_map<geometry::NFPKey, Clipper2Lib::Paths64> m_nfpCache; 

    virtual pattern::NestingResult runAlgorithm() = 0;

    pattern::NestingResult runSingleBottomLeft(std::vector<pattern::NestedItem>& items);
    virtual void prepareNestingItems(std::vector<pattern::PatternPiece>& patternPieces);
public:
    explicit AlgorithmBase(int64_t width = 1400000, int64_t length = 100000000);
    explicit AlgorithmBase(pattern::Fabric fabric);
    virtual ~AlgorithmBase() = default;

    const std::unordered_map<int64_t, Clipper2Lib::Paths64>& getIFPCache() const { return m_ifpCache; };
    const std::unordered_map<geometry::NFPKey, Clipper2Lib::Paths64>& getNFPCache() const { return m_nfpCache; };

    pattern::NestingResult execute(std::vector<pattern::PatternPiece>& patternPieces, timing::PhaseTimings* timings = nullptr);
};

} // namespace pmg::nesting