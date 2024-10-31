#include "ConvexHull.hpp"

class MonotoneChain : public ConvexHull {
public:
    MonotoneChain() : ConvexHull() {}

    vector<Point> convexHull(vector<Point> &points) override {
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

};