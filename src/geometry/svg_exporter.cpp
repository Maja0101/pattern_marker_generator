#include <fstream>
#include <iomanip>
#include <array>

#include "geometry/svg_exporter.hpp"
#include "nesting/nested_item.hpp"
#include "pattern/shapes.hpp"
#include "geometry/geometry_types.hpp"


namespace pmg::geometry {

bool SvgExporter::exportLayout(const std::string& filepath, 
                             const pattern::Fabric& fabric,
                             const std::vector<nesting::NestedItem>& items)
{
    std::ofstream out(filepath);
    if (!out.is_open()) {
        return false;
    }

    out << std::fixed << std::setprecision(3);

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
        << "width=\"" << fabric.getBoundingBox().Width() / 1000 << "mm\" "
        << "height=\"" << fabric.getBoundingBox().Height() / 1000 << "mm\" "
        << "viewBox=\"0 0 " << fabric.getBoundingBox().Width() / 1000 << " " << fabric.getBoundingBox().Height() / 1000 << "\">\n\n";

    out << "  <!-- Sheet Frame -->\n";
    out << "  <g transform=\"translate(0, " << fabric.getBoundingBox().Height() / 1000 << ") scale(1, -1)\">\n";
    out << "  <polygon points=\"";
    for (size_t i = 0; i < fabric.getBoundary().size(); ++i) {
        out << fabric.getBoundary()[i].x / 1000 << "," << fabric.getBoundary()[i].y / 1000;
        if (i + 1 < fabric.getBoundary().size()) {
            out << " ";
        }
    }
    out << "\" fill=\"#FAF7F7\" stroke=\"#404040\" stroke-width=\"0.5\" />\n\n";
    out << "  </g>\n\n";

    out << "  <!-- Placed Pieces -->\n";
    const char* strokeColor = "#8046E1";
    const char* borderColor = "#B84A8D";
    const float borderWidth = 20.0f;

    size_t idCounter = 0;
    for (const auto& item : items) {
        if (item.isLocked()) {
            const auto* variant = item.getVariant();
            if (!variant) continue;

            const auto pos = item.getPosition();

            std::ostringstream pointsStream;
            for (const auto& pt : variant->getBoundary()) {
                double worldX = static_cast<double>(pos.x + pt.x) / 1000.0;
                double worldY = static_cast<double>(pos.y + pt.y) / 1000.0;
                double svgY = static_cast<double>(fabric.getBoundingBox().Height() / 1000) - worldY;

                pointsStream << worldX << "," << svgY << " ";
            }
            const std::string points = pointsStream.str();
            std::string clipId = "clip_" + std::to_string(idCounter++);

            // Output clipPath definition
            out << "  <clipPath id=\"" << clipId << "\">\n"
                << "    <polygon points=\"" << points << "\" />\n"
                << "  </clipPath>\n";

            // Output polygon referencing the clipPath
            out << "  <polygon points=\"" << points << "\" fill=\"" << strokeColor << "\" fill-opacity=\"0.45\" "
                << "stroke=\"" << borderColor << "\" stroke-width=\"" << (borderWidth * 2.0) << "\" "
                << "clip-path=\"url(#" << clipId << ")\" />\n";
        }
    }

    out << "</svg>\n";
    out.close();

    return true;
}

} // namespace pmg::geometry