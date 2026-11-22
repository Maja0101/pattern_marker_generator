#pragma once

#include <vector>

#include <clipper2/clipper.h>

#include "pattern/shapes.hpp"
#include "pattern/nested_item.hpp"
#include "pattern/nesting_result.hpp"
#include "geometry/nfp_key.hpp"

namespace pmg::nesting {

class BottomLeft {
private:
    pattern::Fabric m_fabric;

    std::unordered_map<int64_t, Clipper2Lib::Paths64>& m_ifpCache;
    std::unordered_map<geometry::NFPKey, Clipper2Lib::Paths64>& m_nfpCache;

    Clipper2Lib::Paths64 m_placedPiecesPolygons;
    std::vector<Clipper2Lib::Rect64> m_placedPiecesBoxes;

    void sortCandidatePoints(std::vector<Clipper2Lib::Point64>& candidatePoints, const pattern::NestedItem& currentItem, double jumpRatio = 0.25);
    bool findPosition(pattern::NestedItem& item, const std::vector<pattern::NestedItem>& items);
public:
    BottomLeft(pattern::Fabric fabric, 
        std::unordered_map<int64_t, Clipper2Lib::Paths64>& ifpCache, 
        std::unordered_map<geometry::NFPKey, Clipper2Lib::Paths64>& nfpCache);

    pattern::NestingResult performNesting(std::vector<pattern::NestedItem>& items);
};

} // namespace pmg::nesting