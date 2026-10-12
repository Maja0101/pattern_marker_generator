#include <fstream>
#include <iomanip>
#include <array>

#include "geometry/svg_exporter.hpp"
#include "nesting/nested_item.hpp"
#include "pattern/shapes.hpp"
#include "geometry/geometry_types.hpp"


namespace pmg::geometry {

bool SvgExporter::exportLayout(const std::string& filepath, 
                             int64_t fabricWidth,
                             int64_t fabricHeight,
                             const std::vector<nesting::NestedItem>& items)
{
    std::ofstream out(filepath);
    if (!out.is_open()) {
        return false;
    }

    out << std::fixed << std::setprecision(3);

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
        << "width=\"" << fabricWidth << "mm\" "
        << "height=\"" << fabricHeight << "mm\" "
        << "viewBox=\"0 0 " << fabricWidth << " " << fabricHeight << "\">\n\n";

    out << "  <!-- Sheet Frame -->\n";
    out << "  <rect width=\"" << fabricWidth << "\" height=\"" << fabricHeight << "\" "
        << "fill=\"#FAF7F7\" stroke=\"#404040\" stroke-width=\"1.5\" />\n\n";

    out << "  <!-- Placed Pieces -->\n";
    const char* strokeColor = "#8046E1";

    for (const auto& item : items) {
        const auto* variant = item.getVariant();
        if (!variant) continue;

        const auto pos = item.getPosition();

        out << "  <polygon points=\"";

        for (const auto& pt : variant->getBoundary()) {
            double worldX = static_cast<double>(pos.x + pt.x) / 1000.0;
            double worldY = static_cast<double>(pos.y + pt.y) / 1000.0;

            double svgY = static_cast<double>(fabricHeight) - worldY;

            out << worldX << "," << svgY << " ";
        }

        out << "\" fill=\"" << strokeColor << "\" fill-opacity=\"0.45\" "
            << "stroke=\"" << strokeColor << "\" stroke-width=\"0.8\" />\n";
    }

    out << "</svg>\n";
    out.close();

    return true;
}

} // namespace pmg::geometry