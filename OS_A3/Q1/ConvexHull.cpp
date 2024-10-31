#include <complex>
#include "ConvexHull.hpp"

float ConvexHull::calculateArea(vector<Point> &points) {
    size_t n = points.size();

    float area = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const Point& p1 = points[i];
        const Point& p2 = points[(i + 1) % n]; // Wrap around to the first point

        area += p1.getX() * p2.getY() - p1.getY() * p2.getX();
    }

    return std::abs(area) / 2.0;
}
