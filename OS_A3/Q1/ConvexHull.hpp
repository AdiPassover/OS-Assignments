#include <vector>
#include <algorithm>
#include <stack>
#include "Point.hpp"

#ifndef OS_A3_CONVEXHULL_HPP
#define OS_A3_CONVEXHULL_HPP

using std::vector;

class ConvexHull {
private:
    static float calculateArea(vector<Point> &points);

public:
    virtual vector<Point> convexHull(vector<Point> &points) = 0;

    float convexHullArea(vector<Point> &points) {
        vector<Point> convex = convexHull(points);
        return calculateArea(convex);
    }

    virtual ~ConvexHull() = default;

};

#endif //OS_A3_CONVEXHULL_HPP
