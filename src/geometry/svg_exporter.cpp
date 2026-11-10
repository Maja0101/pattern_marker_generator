#include <fstream>
#include <iomanip>

#include "geometry/svg_exporter.hpp"
#include "nesting/nested_item.hpp"
#include "pattern/shapes.hpp"

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
    const double BORDER_WIDTH = 1.0;
    const int SCALE_TO_MM = 1000;
    const double GAP_WIDTH = static_cast<double>(includedCuttingAllowance) / SCALE_TO_MM;
    const double STRIPE_WIDTH = static_cast<double>(includedSeamAllowance) / SCALE_TO_MM;

    out << std::fixed << std::setprecision(3);

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
        << "width=\"" << fabric.getBoundingBox().Height() / SCALE_TO_MM << "mm\" "
        << "height=\"" << fabric.getBoundingBox().Width() / SCALE_TO_MM << "mm\" "
        << "viewBox=\"0 0 " << fabric.getBoundingBox().Height() / SCALE_TO_MM << " " << fabric.getBoundingBox().Width() / SCALE_TO_MM << "\">\n\n";

    out << "  <!-- Sheet Frame -->\n";
    out << "  <g transform=\"rotate(90) translate(0, -" << fabric.getBoundingBox().Height() / SCALE_TO_MM << ") \">\n";
    out << "  <polygon points=\"";
    for (size_t i = 0; i < fabric.getBoundary().size(); ++i) {
        out << fabric.getBoundary()[i].x / SCALE_TO_MM << "," << fabric.getBoundary()[i].y / SCALE_TO_MM;
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

            const auto POS = item.getPosition();

            // Change the coordinates of points for display
            std::ostringstream pointsStream;
            for (const auto& pt : variant->getBoundary(item.isFlipped())) {
                double worldX = static_cast<double>(POS.x + pt.x) / SCALE_TO_MM;
                double worldY = static_cast<double>(POS.y + pt.y) / SCALE_TO_MM;
                double svgY = (static_cast<double>(fabric.getBoundingBox().Height()) / SCALE_TO_MM) - worldY;

                pointsStream << worldX << "," << svgY << " ";
            }
            const std::string POINTS = pointsStream.str();
            std::string clipId = "clip_" + std::to_string(idCounter++);

            // Calculate total strokes width
            const double MULTIPLIER = 2.0;
            double strokeStripe      = MULTIPLIER * (BORDER_WIDTH + GAP_WIDTH + STRIPE_WIDTH);
            double strokeGap         = MULTIPLIER * (BORDER_WIDTH + GAP_WIDTH);
            double strokeOuterBorder = MULTIPLIER * BORDER_WIDTH;

            // Output clipPath definition
            out << "  <clipPath id=\"" << clipId << "\">\n"
                << "    <polygon points=\"" << POINTS << "\" />\n"
                << "  </clipPath>\n";

            // Output polygon referencing the clipPath
            out << "  <g clip-path=\"url(#" << clipId << ")\">\n"
            << "    <polygon points=\"" << POINTS << "\" fill=\"" << strokeColor << "\" fill-opacity=\"0.45\" />\n"
            << "    <polygon points=\"" << POINTS << R"(" fill="none" stroke=")" << strokeColor << R"(" fill-opacity="0.45" stroke-width=")" << strokeStripe << "\" />\n"
            << "    <polygon points=\"" << POINTS << R"(" fill="none" stroke=")" << backgroundColor << "\" stroke-width=\"" << strokeGap << "\" />\n"
            << "    <polygon points=\"" << POINTS << R"(" fill="none" stroke=")" << borderColor << "\" stroke-width=\"" << strokeOuterBorder << "\" />\n"
            << "  </g>\n";
        }
    }

    out << "  </g>\n\n";
    out << "</svg>\n";
    out.close();

    return true;
}

} // namespace pmg::geometry