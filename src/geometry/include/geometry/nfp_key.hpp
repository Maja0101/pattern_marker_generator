#pragma once

#include <cstdint>
#include <cstddef>
#include <functional>

namespace pmg::geometry {

struct NFPKey {
    int64_t variantIdA;
    int64_t variantIdB;

    bool operator==(const NFPKey& other) const {
        return variantIdA == other.variantIdA && variantIdB == other.variantIdB;
    }
};


} // namespace pmg::geometry 

template<>
struct std::hash<pmg::geometry::NFPKey> {
    std::size_t operator()(const pmg::geometry::NFPKey& k) const noexcept {
        std::size_t h1 = std::hash<int64_t>{}(k.variantIdA);
        std::size_t h2 = std::hash<int64_t>{}(k.variantIdB);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2)); // formula for combining hash values
    }
};