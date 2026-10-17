#pragma once

#include <unordered_map>
#include <vector>

#include <clipper2/clipper.h>

#include "geometry/nfp_key.hpp"
#include "pattern/shapes.hpp"

namespace pmg::geometry::calculations {

std::unordered_map<int, Clipper2Lib::Paths64> calculateIFPCache(const pattern::Fabric& fabric, const std::vector<pattern::PieceVariant>& variants); 
std::unordered_map<NFPKey, Clipper2Lib::Paths64> calculateNFPCache(const std::vector<pattern::PieceVariant>& variants);

Clipper2Lib::Paths64 buildIFPfor(const pattern::Fabric& fabric, const pattern::PieceVariant& variant);
Clipper2Lib::Paths64 buildNFPfor(const pattern::PieceVariant& variantA, const pattern::PieceVariant& variantB);

} // namespace pmg::geometry::calculations