#include "geometry/ifp_pocket_calculations.hpp"
#include "geometry/calculations.hpp"

namespace pmg::geometry::calculations::utils {

std::vector<Point64> calculateConvexHull(const std::vector<Point64> &polygon) {
    int n = polygon.size();
    if (n <= 3) return polygon;

    // Sort the point in the polygon lexicographically
    std::vector<Point64> sorted = polygon;
    std::sort(sorted.begin(), sorted.end());

    // Prepare the output vector
    std::vector<Point64> hull;
    hull.reserve(2*n);
    
    // Build lower hull
    for (int i = 0; i < n; ++i) {
        while (hull.size() >= 2 && crossProduct(hull[hull.size() - 2], hull.back(), sorted[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(sorted[i]);
    }

    // Build upper hull
    size_t lowerSize = hull.size();
    for (int i = n - 2; i >= 0; --i) {
        while (hull.size() > lowerSize && crossProduct(hull[hull.size() - 2], hull.back(), sorted[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(sorted[i]);
    }

    // Remove the last point that is the same as the first
    hull.pop_back();

    return hull;
}

} // namespace pmg::geometry::calculations::utils