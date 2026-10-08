#pragma once

#include "geometry/geometry_types.hpp"
#include "pattern/piece_variant.hpp"

namespace pmg::nesting {

class PlacedInstance {
private:
    geometry::Point64 position;
    const pattern::PieceVariant* variantRef;

public:
    PlacedInstance(geometry::Point64 pos, const pattern::PieceVariant* variant) noexcept;

    std::string toString() const;

    geometry::Point64 getPosition() const noexcept { return position; }
    const pattern::PieceVariant* getVariant() const noexcept { return variantRef; }
};

} // namespace pmg::nesting