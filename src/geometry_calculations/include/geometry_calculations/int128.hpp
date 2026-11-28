#pragma once

#include <compare>
#include <cstdint>
#include <format>

#if defined(__SIZEOF_INT128__)

namespace pmg::geometry {

using int128_t = __int128_t;

inline int128_t mul128(int64_t a, int64_t b) noexcept {
    return static_cast<int128_t>(a) * b;
}

} // namespace pmg::geometry

#elif defined(_MSC_VER)
#include <intrin.h>

namespace pmg::geometry {

struct int128_t {
    uint64_t lo = 0;
    int64_t hi = 0;

    constexpr int128_t() noexcept = default;
    constexpr int128_t(int64_t v) noexcept
        : lo(static_cast<uint64_t>(v)), hi(v < 0 ? -1 : 0) {}
    constexpr int128_t(uint64_t l, int64_t h) noexcept : lo(l), hi(h) {}

    constexpr int128_t operator-() const noexcept {
        uint64_t nlo = ~lo + 1;
        int64_t nhi = ~hi + (nlo == 0 ? 1 : 0);
        return {nlo, nhi};
    }

    constexpr int128_t operator+(const int128_t& o) const noexcept {
        uint64_t rlo = lo + o.lo;
        int64_t rhi = hi + o.hi + (rlo < lo ? 1 : 0);
        return {rlo, rhi};
    }

    constexpr int128_t operator-(const int128_t& o) const noexcept {
        return *this + (-o);
    }

    constexpr bool operator==(const int128_t&) const noexcept = default;
    constexpr auto operator<=>(const int128_t& o) const noexcept {
        if (auto cmp = hi <=> o.hi; cmp != 0) return cmp;
        return lo <=> o.lo;
    }

    constexpr int128_t operator/(int) const noexcept {
        return {(lo >> 1) | (static_cast<uint64_t>(hi) << 63), hi >> 1};
    }

    int128_t operator/(const int128_t& d) const noexcept {
        int64_t rem = 0;
        int64_t q = _div128(hi, static_cast<int64_t>(lo), static_cast<int64_t>(d.lo), &rem);
        return int128_t(q);
    }

    explicit constexpr operator int64_t() const noexcept {
        return static_cast<int64_t>(lo);
    }
};

inline int128_t mul128(int64_t a, int64_t b) noexcept {
    int64_t hi = 0;
    int64_t lo = _mul128(a, b, &hi);
    return {static_cast<uint64_t>(lo), hi};
}

inline int128_t operator*(int64_t a, const int128_t& b) noexcept {
    uint64_t ua = a < 0 ? ~static_cast<uint64_t>(a) + 1 : static_cast<uint64_t>(a);
    int128_t ub = b < 0 ? -b : b;

    uint64_t hi_lo = 0;
    uint64_t lo = _umul128(ua, ub.lo, &hi_lo);
    int64_t hi = static_cast<int64_t>(hi_lo + ua * static_cast<uint64_t>(ub.hi));

    int128_t res{lo, hi};
    return ((a < 0) != (b < 0)) ? -res : res;
}

inline int128_t operator*(const int128_t& a, int64_t b) noexcept {
    return b * a;
}

} // namespace pmg::geometry

template <>
struct std::formatter<pmg::geometry::int128_t> : std::formatter<int64_t> {
    auto format(const pmg::geometry::int128_t& v, auto& ctx) const {
        return std::formatter<int64_t>::format(static_cast<int64_t>(v), ctx);
    }
};

#endif
