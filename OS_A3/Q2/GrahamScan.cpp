#include "ConvexHull.hpp"

class GrahamScan : public ConvexHull {
public:
    GrahamScan() : ConvexHull() {}

    vector<Point> convexHull(vector<Point>& points) override {

        Point p0 = *min_element(points.begin(), points.end());
        sort(points.begin(), points.end(), [&p0](const Point &a, const Point &b) {
            Orientation orientation = p0.orientation(a, b);
            if (orientation == COLLINEAR) // If points are collinear, the one closer to the p0 should come first
                return p0.distance(a) < p0.distance(b);
            return orientation > 0;  // Sort counterclockwise
        });

        vector<Point> hull;
        hull.push_back(points[0]);
        hull.push_back(points[1]);
        for (size_t i = 2; i < points.size(); i++) {
            while (hull.size() > 1 && hull[hull.size() - 2].orientation(hull.back(), points[i]) <= 0)
                hull.pop_back();
            hull.push_back(points[i]);
        }

        return hull;
    }

};