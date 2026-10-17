#include "geometry/nfp_ifp_calculations.hpp"

namespace pmg::geometry::calculations {

std::unordered_map<int, Clipper2Lib::Paths64> calculateIFPCache(const pattern::Fabric &fabric, const std::vector<pattern::PieceVariant> &variants)
{
    std::unordered_map<int, Clipper2Lib::Paths64> ifpCache;
    for (const auto& variant : variants) {
        if (variant.getRequiredQuantity() > 0) {
            ifpCache[variant.getId()] = buildIFPfor(fabric, variant);
        }
    }
    return ifpCache;
}

std::unordered_map<NFPKey, Clipper2Lib::Paths64> calculateNFPCache(const std::vector<pattern::PieceVariant> &variants)
{
    std::unordered_map<NFPKey, Clipper2Lib::Paths64> nfpCache;
    for (const auto& variantA : variants) {
        if (variantA.getRequiredQuantity() > 0) {
            for (const auto& variantB : variants) {
                if (variantB.getRequiredQuantity() > 0) {
                    NFPKey key{ variantA.getId(), variantB.getId() };
                    nfpCache[key] = buildNFPfor(variantA, variantB);
                }
            }
        }
    }
    return std::unordered_map<NFPKey, Clipper2Lib::Paths64>();
}

Clipper2Lib::Paths64 buildIFPfor(const pattern::Fabric &fabric, const pattern::PieceVariant &variant)
{
    // TODO calculate IFP

    return Clipper2Lib::Paths64();
}

Clipper2Lib::Paths64 buildNFPfor(const pattern::PieceVariant &variantA, const pattern::PieceVariant &variantB)
{
    // TODO calculate NFP

    return Clipper2Lib::Paths64();
}

} // namespace pmg::geometry::calculations