#include "GrahamScan.cpp"
#include "MonotoneChain.cpp"
#include <iostream>
#include <string.h>

using std::cout;
using std::cin;
using std::endl;

void inputPoints(vector<Point> &p) {
    float x, y;
    for (size_t i = 0; i < p.size(); i++) {
//        cout << "Enter the x and y coordinates of point (separated by comma) " << i + 1 << ": ";
        if (fscanf(stdin, "%f,%f", &x, &y) != 2)
            throw std::runtime_error("Invalid input");
        p[i] = Point(x, y);
    }
}

vector<Point> inputPoints() {
    size_t numPoints;
//    cout << "Enter the number of points: ";
    if (!(cin >> numPoints))
        throw std::runtime_error("Invalid input");
    if (numPoints < 3)
        throw std::runtime_error("At least 3 points are required");

    vector<Point> points(numPoints);
    inputPoints(points);

    return points;
}

vector<Point> randomPoints(unsigned int numPoints, int seed) {
    srand(seed);
    vector<Point> points(numPoints);
    for (unsigned int i = 0; i < numPoints; i++) {
        points[i] = Point(rand() % 10000, rand() % 10000);
    }
    return points;
}

int main()
{
    unsigned int numPoints = 1000000;
    int seed = 42;
    vector<Point> points = randomPoints(numPoints, seed);

    ConvexHull *ch = new GrahamScan();
    cout << "Graham scan convex hull area of " << numPoints << " random points for seed " << seed << ": ";
    cout << ch->convexHullArea(points) << endl;
    delete ch;

    ch = new MonotoneChain();
    cout << "Monotone chain convex hull area of " << numPoints << " random points for seed " << seed << ": ";
    cout << ch->convexHullArea(points) << endl;
    delete ch;

    return 0;
}