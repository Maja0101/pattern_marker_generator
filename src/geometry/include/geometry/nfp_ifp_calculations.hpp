#pragma once

#include <unordered_map>
#include <vector>

#include <clipper2/clipper.h>

#include "geometry/nfp_key.hpp"
#include "geometry/calculations.hpp"
#include "pattern/shapes.hpp"

namespace pmg::geometry::calculations {

std::unordered_map<int64_t, Clipper2Lib::Paths64> calculateIFPCache(const pattern::Fabric& fabric, const std::vector<pattern::PieceVariant>& variants); 
std::unordered_map<NFPKey, Clipper2Lib::Paths64> calculateNFPCache(const std::vector<pattern::PieceVariant>& variants);

} // namespace pmg::geometry::calculations