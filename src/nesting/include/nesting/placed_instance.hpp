#pragma once

#include "geometry/geometry_types.hpp"
#include "pattern/piece_variant.hpp"

namespace pmg::nesting {

class PlacedInstance {
private:
    geometry::Point64 m_position;
    const pattern::PieceVariant* m_variantRef;

public:
    PlacedInstance(geometry::Point64 pos, const pattern::PieceVariant* variant) noexcept;

    std::string toString() const;

    geometry::Point64 getPosition() const noexcept { return m_position; }
    const pattern::PieceVariant* getVariant() const noexcept { return m_variantRef; }
};

} // namespace pmg::nesting