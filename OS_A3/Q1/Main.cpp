#include "GrahamScan.cpp"
#include "MonotoneChain.cpp"
#include <iostream>
#include <string.h>
#include <stdio.h>

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

int main()
{
    vector<Point> points = inputPoints();

    ConvexHull *ch = new MonotoneChain();
    cout << ch->convexHullArea(points) << endl;
    delete ch;

    return 0;
}