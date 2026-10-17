#include "geometry/nfp_ifp_calculations.hpp"
#include "geometry/calculations.hpp"

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

Clipper2Lib::Paths64 buildIFPfor(const pattern::Fabric &fabric, const pattern::PieceVariant &variant, int64_t margin)
{
    // TODO calculate IFP
    Clipper2Lib::Rect64 fabricBounds = fabric.getBoundingBox();
    Clipper2Lib::Path64 fabricPath = fabric.getPath();

    if (isAARectangle(fabricPath, fabricBounds)) {
        // Fast IFP calculation for rectangular fabric
        Clipper2Lib::Rect64 variantBounds = variant.getBoundingBox();

        int64_t minX = fabricBounds.left - variantBounds.left;
        int64_t maxX = fabricBounds.right - variantBounds.right;
        int64_t minY = fabricBounds.bottom - variantBounds.bottom;
        int64_t maxY = fabricBounds.top - variantBounds.top;

        // IFP empty when variant bigger than fabric
        if (minX > maxX || minY > maxY) {
            return Clipper2Lib::Paths64{};
        }

        Clipper2Lib::Path64 ifp = {
            {minX, minY},
            {maxX, minY},
            {maxX, maxY},
            {minX, maxY}
        };

        return Clipper2Lib::Paths64{ ifp };
    } else {
        // Detailed calculation for irregular fabric

        // IFP can be calculated as subtracting the complement of the fabric and the Minkowski sum with the inverted shape from the fabric

        // Construction of the finite complement of the fabric
        Clipper2Lib::Path64 outerBox = {
            {fabricBounds.left - margin, fabricBounds.bottom - margin},
            {fabricBounds.right + margin, fabricBounds.bottom - margin},
            {fabricBounds.right + margin, fabricBounds.top + margin},
            {fabricBounds.left - margin, fabricBounds.top + margin},
        };

        Clipper2Lib::Paths64 fabricComplement = Clipper2Lib::Difference(
            Clipper2Lib::Paths64{ outerBox },
            Clipper2Lib::Paths64{ fabricPath },
            Clipper2Lib::FillRule::NonZero
        );

        // Construction of inverted shape
        std::vector<Clipper2Lib::Path64> decomposedSubShapes; // TODO calculate sub shapes -> change concave shape to multiple convex shapes
        std::vector<Clipper2Lib::Path64> invSubShapes;
        invSubShapes.reserve(decomposedSubShapes.size()); 
        for (const auto& subShape : decomposedSubShapes) {
            invSubShapes.push_back(invertPath(subShape));
        }

        // Minkowski sums
        Clipper2Lib::Paths64 nfp;
        for (const auto& fabricComplementFragment : fabricComplement) {
            for (const auto& invSubShape : invSubShapes) {
                Clipper2Lib::Paths64 pairNfp = Clipper2Lib::MinkowskiSum(
                    fabricComplementFragment,
                    invSubShape,
                    true
                );

                nfp = Clipper2Lib::Union(
                    nfp,
                    pairNfp,
                    Clipper2Lib::FillRule::NonZero
                );
            }
        }

        // Subtract NFP from fabric path to get IFP
        Clipper2Lib::Paths64 ifp = Clipper2Lib::Difference(
            Clipper2Lib::Paths64{ fabricPath },
            nfp,
            Clipper2Lib::FillRule::NonZero
        );

        return Clipper2Lib::SimplifyPaths(ifp, 0.0); // Remove redundant vertexes (wihout changing the geometric shape)
    }

}

Clipper2Lib::Paths64 buildNFPfor(const pattern::PieceVariant &variantA, const pattern::PieceVariant &variantB)
{
    // TODO calculate NFP

    return Clipper2Lib::Paths64();
}

} // namespace pmg::geometry::calculations