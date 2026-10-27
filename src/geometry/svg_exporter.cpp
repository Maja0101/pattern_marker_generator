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
                               const std::vector<nesting::NestedItem>& items,
                               int64_t includedSeamAllowance, 
                               int64_t includedCuttingAllowance)
{
    std::ofstream out(filepath);
    if (!out.is_open()) {
        return false;
    }

    const char* strokeColor = "#D9D9D9";
    const char* backgroundColor = "#FAF7F7";
    const char* borderColor = "#301560";
    const double borderWidth = 1.0;
    const double gapWidth = static_cast<double>(includedCuttingAllowance) / 1000;
    const double stripeWidth = static_cast<double>(includedSeamAllowance) / 1000;

    out << std::fixed << std::setprecision(3);

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
        << "width=\"" << fabric.getBoundingBox().Height() / 1000 << "mm\" "
        << "height=\"" << fabric.getBoundingBox().Width() / 1000 << "mm\" "
        << "viewBox=\"0 0 " << fabric.getBoundingBox().Height() / 1000 << " " << fabric.getBoundingBox().Width() / 1000 << "\">\n\n";

    out << "  <!-- Sheet Frame -->\n";
    out << "  <g transform=\"rotate(90) translate(0, -" << fabric.getBoundingBox().Height() / 1000 << ") \">\n";
    out << "  <polygon points=\"";
    for (size_t i = 0; i < fabric.getBoundary().size(); ++i) {
        out << fabric.getBoundary()[i].x / 1000 << "," << fabric.getBoundary()[i].y / 1000;
        if (i + 1 < fabric.getBoundary().size()) {
            out << " ";
        }
    }
    out << "\" fill=\"" << backgroundColor << "\" stroke=\"#404040\" stroke-width=\"0.5\" />\n\n";
    
    out << "  <!-- Placed Pieces -->\n";
    
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

            // Calculate total strokes width
            double strokeStripe = 2.0 * (borderWidth + gapWidth + stripeWidth);
            double strokeGap         = 2.0 * (borderWidth + gapWidth);
            double strokeOuterBorder      = 2.0 * borderWidth;

            // Output clipPath definition
            out << "  <clipPath id=\"" << clipId << "\">\n"
                << "    <polygon points=\"" << points << "\" />\n"
                << "  </clipPath>\n";

            // Output polygon referencing the clipPath
            out << "  <g clip-path=\"url(#" << clipId << ")\">\n"
            << "    <polygon points=\"" << points << "\" fill=\"" << strokeColor << "\" fill-opacity=\"0.45\" />\n"
            << "    <polygon points=\"" << points << "\" fill=\"none\" stroke=\"" << strokeColor << "\" fill-opacity=\"0.45\" stroke-width=\"" << strokeStripe << "\" />\n"
            << "    <polygon points=\"" << points << "\" fill=\"none\" stroke=\"" << backgroundColor << "\" stroke-width=\"" << strokeGap << "\" />\n"
            << "    <polygon points=\"" << points << "\" fill=\"none\" stroke=\"" << borderColor << "\" stroke-width=\"" << strokeOuterBorder << "\" />\n"
            << "  </g>\n";
        }
    }

    out << "  </g>\n\n";
    out << "</svg>\n";
    out.close();

    return true;
}

} // namespace pmg::geometry