#include <spdlog/spdlog.h>

#include "geometry/nfp_ifp_calculations.hpp"
#include "geometry/calculations.hpp"
#include "utils/config.hpp"

namespace pmg::geometry::calculations {

std::unordered_map<int64_t, Clipper2Lib::Paths64> calculateIFPCache(const pattern::Fabric &fabric, const std::vector<pattern::PieceVariant> &variants)
{
    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("Number of variants: {}", variants.size()));
    }

    std::unordered_map<int64_t, Clipper2Lib::Paths64> ifpCache;
    for (const auto& variant : variants) {
        if (variant.getRequiredQuantity() > 0) {
            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO(std::format("Variant no. {}", variant.getGlobalId()));
            }
            ifpCache[variant.getGlobalId()] = buildIFPfor(fabric, variant);
        }
    }
    return ifpCache;
}

std::unordered_map<NFPKey, Clipper2Lib::Paths64> calculateNFPCache(const std::vector<pattern::PieceVariant> &variants)
{
    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("Number of variants: {}", variants.size()));
    }

    std::unordered_map<NFPKey, Clipper2Lib::Paths64> nfpCache;
    for (const auto& variantA : variants) {
        if (variantA.getRequiredQuantity() > 0) {
            for (const auto& variantB : variants) {
                if (variantB.getRequiredQuantity() > 0) {
                    if constexpr (config::DEBUG_MODE_DETAILED) {
                        SPDLOG_INFO(std::format("Variants no. {} {}", variantA.getGlobalId(), variantB.getGlobalId()));
                    }
                    NFPKey key{ variantA.getGlobalId(), variantB.getGlobalId() };
                    nfpCache[key] = buildNFPfor(variantA, variantB);
                }
            }
        }
    }
    return nfpCache;
}

Clipper2Lib::Paths64 buildIFPfor(const pattern::Fabric &fabric, const pattern::PieceVariant &variant, int64_t margin)
{
    const Clipper2Lib::Rect64& fabricBounds = fabric.getBoundingBox();
    const Clipper2Lib::Path64& fabricPath = fabric.getPath();

    if constexpr (config::DEBUG_MODE) {
        SPDLOG_INFO(std::format("Variants no. {} {}", variant.getId(), variant.getGlobalId()));
    }

    if (isAARectangle(fabricPath, fabricBounds)) {
        if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO("Rectangle fabric");
        }

        // Fast IFP calculation for rectangular fabric
        const Clipper2Lib::Rect64& variantBounds = variant.getBoundingBox();

        int64_t minX = fabricBounds.left - variantBounds.left;
        int64_t maxX = fabricBounds.right - variantBounds.right;
        int64_t minY = fabricBounds.top - variantBounds.top; // top - min in clipper
        int64_t maxY = fabricBounds.bottom - variantBounds.bottom;

        if constexpr (config::DEBUG_MODE) {
            SPDLOG_INFO(std::format("x: {} - {} y: {} - {}", minX, maxX, minY, maxY));
        }

        // IFP empty when variant bigger than fabric
        if (minX > maxX || minY > maxY) {
            if constexpr (config::DEBUG_MODE) {
                SPDLOG_INFO("IFP empty, variant to big");
            }
            return Clipper2Lib::Paths64{};
        }

        Clipper2Lib::Path64 ifp = {
            {minX, minY},
            {maxX, minY},
            {maxX, maxY},
            {minX, maxY}
        };

        if constexpr (config::DEBUG_MODE) {
            for (const auto& p : ifp) {
                SPDLOG_INFO(std::format("IFP: ({}, {})", p.x, p.y));
            }
        }

        return Clipper2Lib::Paths64{ ifp };
    } 

    if constexpr (config::DEBUG_MODE) {
        SPDLOG_INFO("Irregular  fabric");
    }

    // Detailed calculation for irregular fabric

    // IFP can be calculated as subtracting the complement of the fabric and the Minkowski sum with the inverted shape from the fabric

    // Construction of the finite complement of the fabric
    Clipper2Lib::Path64 outerBox = {
        {fabricBounds.left - margin, fabricBounds.top - margin}, // minX - margin, minY - margin
        {fabricBounds.right + margin, fabricBounds.top - margin}, // maxX + margin, minY - margin
        {fabricBounds.right + margin, fabricBounds.bottom + margin}, // maxX + margin, maxY + margin
        {fabricBounds.left - margin, fabricBounds.bottom + margin}, // minX - margin, maxY + margin
    };

    if constexpr (config::DEBUG_MODE) {
        for (const auto& p : outerBox) {
            SPDLOG_INFO(std::format("outerBox: ({}, {})", p.x, p.y));
        }
    }

    Clipper2Lib::Paths64 fabricComplement = Clipper2Lib::Difference(
        Clipper2Lib::Paths64{ outerBox },
        Clipper2Lib::Paths64{ fabricPath },
        Clipper2Lib::FillRule::NonZero
    );

    if constexpr (config::DEBUG_MODE) {
        for (const auto& path : fabricComplement) {
            SPDLOG_INFO("path in fabricComplement");
            for (const auto& p : path) {
                SPDLOG_INFO(std::format("fabricComplement: ({}, {})", p.x, p.y));
            }
        }
    }

    // Minkowski sums with the inverted shape
    const std::vector<Clipper2Lib::Path64>& decomposedSubShapes = variant.getDecomposedShape(); 

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

    if constexpr (config::DEBUG_MODE) {
        SPDLOG_INFO(std::format("Number of sub nfps: {}", subNFPs.size()));
    }

    Clipper2Lib::Paths64 nfp = Clipper2Lib::Union(
        subNFPs,
        Clipper2Lib::FillRule::NonZero
    );

    if constexpr (config::DEBUG_MODE) {
        for (const auto& path : subNFPs) {
            SPDLOG_INFO("path in subNFPs");
            for (const auto& p : path) {
                SPDLOG_INFO(std::format("subNFPs: ({}, {})", p.x, p.y));
            }
        }
    }

    // Subtract NFP from fabric path to get IFP
    Clipper2Lib::Paths64 ifp = Clipper2Lib::Difference(
        Clipper2Lib::Paths64{ fabricPath },
        nfp,
        Clipper2Lib::FillRule::NonZero
    );

    if constexpr (config::DEBUG_MODE) {
        for (const auto& path : ifp) {
            SPDLOG_INFO("path in ifp");
            for (const auto& p : path) {
                SPDLOG_INFO(std::format("IFP: ({}, {})", p.x, p.y));
            }
        }
    }

    return Clipper2Lib::SimplifyPaths(ifp, 0.0); // Remove redundant vertexes (wihout changing the geometric shape)

}

Clipper2Lib::Paths64 buildNFPfor(const pattern::PieceVariant &variantA, const pattern::PieceVariant &variantB)
{
    // NFP can be calculated as adding the inverted shape B to the shape A 

    if (variantA.getIsConvex() && variantB.getIsConvex()) {
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO(std::format("Both convex: {} {}, {} {}", variantA.getId(), variantA.getGlobalId(), variantB.getId(), variantB.getGlobalId()));
        }
        // Fast NFP calculation for convex shapes
        Clipper2Lib::Path64 invertedB = invertPath(variantB.getPath());

        Clipper2Lib::Paths64 rawNfp = Clipper2Lib::MinkowskiSum(
            variantA.getPath(),
            invertedB,
            true
        );

        Clipper2Lib::Paths64 nfp;
        for (const auto& path : rawNfp) {
            if (Clipper2Lib::IsPositive(path)) {
                nfp.push_back(path);
            }
        }

        if constexpr (config::DEBUG_MODE_DETAILED) {
            for (const auto& path : Clipper2Lib::SimplifyPaths(nfp, 0.0)) {
                SPDLOG_INFO("path in nfp");
                for (const auto& p : path) {
                    SPDLOG_WARN(std::format("NFP: ({}, {})", p.x, p.y));
                }
            }
        }
        
        return Clipper2Lib::SimplifyPaths(nfp, 0.0);

    } 

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("concave: {} {} {}, {} {} {}", variantA.getId(), variantA.getGlobalId(), variantA.getIsConvex(), variantB.getId(), variantB.getGlobalId(), variantB.getIsConvex()));
    }
    
    // Detailed calculation for concave shapes
    const std::vector<Clipper2Lib::Path64>& subShapesA = variantA.getDecomposedShape();
    const std::vector<Clipper2Lib::Path64>& subShapesB = variantB.getDecomposedShape();

    Clipper2Lib::Paths64 subNFPs;
    subNFPs.reserve(subShapesA.size() * subShapesB.size());

    for (const auto& subA : subShapesA) {
        if constexpr (config::DEBUG_MODE_DETAILED) {
            SPDLOG_INFO("A");
        }
        for (const auto& subB : subShapesB) {
            if constexpr (config::DEBUG_MODE_DETAILED) {
                SPDLOG_INFO("B");
            }
            Clipper2Lib::Path64 invertedSubB = invertPath(subB);

            Clipper2Lib::Paths64 pairNfp = Clipper2Lib::MinkowskiSum(
                subA,
                invertedSubB,
                true
            );

            if constexpr (config::DEBUG_MODE_DETAILED) {
                for (const auto& path : pairNfp) {
                    SPDLOG_INFO("path in nfp");
                    for (const auto& p : path) {
                        SPDLOG_WARN(std::format("NFP: ({}, {})", p.x, p.y));
                    }
                }
            }

            subNFPs.insert(subNFPs.end(), pairNfp.begin(), pairNfp.end());
        }
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        SPDLOG_INFO(std::format("Number of sub nfps: {}", subNFPs.size()));
    }

    Clipper2Lib::Paths64 rawNfp = Clipper2Lib::Union(
        subNFPs,
        Clipper2Lib::FillRule::NonZero
    );

    Clipper2Lib::Paths64 nfp;
    for (const auto& path : rawNfp) {
        if (Clipper2Lib::IsPositive(path)) {
            nfp.push_back(path);
        }
    }

    if constexpr (config::DEBUG_MODE_DETAILED) {
        for (const auto& path : Clipper2Lib::SimplifyPaths(nfp, 0.0)) {
            SPDLOG_INFO("path in nfp");
            for (const auto& p : path) {
                SPDLOG_WARN(std::format("NFP: ({}, {})", p.x, p.y));
            }
        }
    }

    return Clipper2Lib::SimplifyPaths(nfp, 0.0);
}

} // namespace pmg::geometry::calculations