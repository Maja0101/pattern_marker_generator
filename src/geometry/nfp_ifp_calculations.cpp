#include "geometry/nfp_ifp_calculations.hpp"
#include "geometry/calculations.hpp"

namespace pmg::geometry::calculations {

std::unordered_map<int64_t, Clipper2Lib::Paths64> calculateIFPCache(const pattern::Fabric &fabric, const std::vector<pattern::PieceVariant> &variants)
{
    std::unordered_map<int64_t, Clipper2Lib::Paths64> ifpCache;
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
    const Clipper2Lib::Rect64& fabricBounds = fabric.getBoundingBox();
    const Clipper2Lib::Path64& fabricPath = fabric.getPath();

    if (isAARectangle(fabricPath, fabricBounds)) {
        // Fast IFP calculation for rectangular fabric
        const Clipper2Lib::Rect64& variantBounds = variant.getBoundingBox();

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
    } 

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

    // Construction of the inverted shape
    const std::vector<Clipper2Lib::Path64>& decomposedSubShapes = variant.getDecomposedShape(); 

    // Minkowski sums
    Clipper2Lib::Paths64 subNFPs;
    subNFPs.reserve(fabricComplement.size() * decomposedSubShapes.size());

    for (const auto& fabricComplementFragment : fabricComplement) {
        for (const auto& subShape : decomposedSubShapes) {
            Clipper2Lib::Paths64 pairNfp = Clipper2Lib::MinkowskiSum(
                fabricComplementFragment,
                invertPath(subShape),
                true
            );

            subNFPs.insert(subNFPs.end(), pairNfp.begin(), pairNfp.end());
        }
    }

    Clipper2Lib::Paths64 nfp = Clipper2Lib::Union(
        subNFPs,
        Clipper2Lib::FillRule::NonZero
    );

    // Subtract NFP from fabric path to get IFP
    Clipper2Lib::Paths64 ifp = Clipper2Lib::Difference(
        Clipper2Lib::Paths64{ fabricPath },
        nfp,
        Clipper2Lib::FillRule::NonZero
    );

    return Clipper2Lib::SimplifyPaths(ifp, 0.0); // Remove redundant vertexes (wihout changing the geometric shape)

}

Clipper2Lib::Paths64 buildNFPfor(const pattern::PieceVariant &variantA, const pattern::PieceVariant &variantB)
{
    // NFP can be calculated as adding the inverted shape B to the shape A 

    if (variantA.getIsConvex() && variantB.getIsConvex()) {
        // Fast IFP calculation for convex shapes
        Clipper2Lib::Path64 invertedB = invertPath(variantB.getPath());
        Clipper2Lib::Paths64 nfp = Clipper2Lib::MinkowskiSum(
            variantA.getPath(),
            invertedB,
            true
        );
        return Clipper2Lib::SimplifyPaths(nfp, 0.0);

    } 
    
    // Detailed calculation for concave shapes
    const std::vector<Clipper2Lib::Path64>& subShapesA = variantA.getDecomposedShape();
    const std::vector<Clipper2Lib::Path64>& subShapesB = variantB.getDecomposedShape();

    Clipper2Lib::Paths64 subNFPs;
    subNFPs.reserve(subShapesA.size() * subShapesB.size());

    for (const auto& subA : subShapesA) {
        for (const auto& subB : subShapesB) {
            Clipper2Lib::Path64 invertedSubB = invertPath(subB);

            Clipper2Lib::Paths64 pairNfp = Clipper2Lib::MinkowskiSum(
                subA,
                invertedSubB,
                true
            );

            subNFPs.insert(subNFPs.end(), pairNfp.begin(), pairNfp.end());
        }
    }

    Clipper2Lib::Paths64 nfp = Clipper2Lib::Union(
        subNFPs,
        Clipper2Lib::FillRule::NonZero
    );

    return Clipper2Lib::SimplifyPaths(nfp, 0.0);
}

} // namespace pmg::geometry::calculations