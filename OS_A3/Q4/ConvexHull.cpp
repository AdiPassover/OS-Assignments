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

vector<Point> ConvexHull::convexHull(vector<Point> &points) {
    size_t n = points.size(), k = 0;
    vector<Point> convex(n);
    std::sort(points.begin(), points.end());

    // Build lower hull
    for (size_t i = 0; i < n; ++i) {
        while (k >= 2 && convex[k - 2].orientation(convex[k - 1], points[i]) <= 0)
            k--;
        convex[k++] = points[i];
    }

    // Build upper hull
    for (size_t i = n-1, t = k+1; i > 0; --i) {
        while (k >= t && convex[k - 2].orientation(convex[k - 1], points[i - 1]) <= 0)
            k--;
        convex[k++] = points[i - 1];
    }

    convex.resize(k - 1);
    return convex;
}
