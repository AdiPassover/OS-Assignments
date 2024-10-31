#include "GrahamScan.cpp"
#include "MonotoneChain.cpp"
#include <iostream>
#include <vector>
#include <cassert>

using std::cout;
using std::endl;
using std::vector;

void printVector(const vector<Point>& points) {
    for (size_t i = 0; i < points.size(); ++i) {
        cout << "(" << points[i].getX() << "," << points[i].getY() << ") ";
    }
    cout << endl;
}

void testConvexHull(ConvexHull* ch, vector<Point>& points, vector<Point>& expected) {
    vector<Point> result = ch->convexHull(points);
    std::sort(result.begin(), result.end());
    std::sort(expected.begin(), expected.end());
    printVector(result);
    printVector(expected);
    cout << result.size() << " " << expected.size() << endl;
    assert(result.size() == expected.size());
    for (size_t i = 0; i < result.size(); ++i) {
        assert(result[i] == expected[i]);
    }
}

int main() {
    vector<Point> points1 = {{0, 0}, {0, 1}, {1, 1}, {2, 0}, {1, 0.5}};
    vector<Point> expected1 = {{0, 0}, {2, 0}, {1, 1}, {0, 1}};

    vector<Point> points2 = {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {0, 4}, {4, 0}};
    vector<Point> expected2 = {{0, 0}, {0, 4}, {4, 4}, {4, 0}};

    vector<Point> points3 = {{0, 0}, {1, 2}, {2, 4}, {3, 6}, {4, 8}, {5, 10}, {6, 12}, {7, 14}, {8, 16}, {9, 18}, {10, 20}};
    vector<Point> expected3 = {{0, 0}, {10, 20}};

    vector<Point> points4 = {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 9}, {10, 10}, {0, 10}, {10, 0}};
    vector<Point> expected4 = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};

    vector<Point> points5 = {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 9}, {10, 10}, {0, 10}, {10, 0}, {5, 15}};
    vector<Point> expected5 = {{0, 0}, {10, 0}, {5, 15}, {0, 10}, {10, 10}};

    ConvexHull* gs = new GrahamScan();
    ConvexHull* mc = new MonotoneChain();

    testConvexHull(gs, points1, expected1);
    testConvexHull(mc, points1, expected1);

    testConvexHull(gs, points2, expected2);
    testConvexHull(mc, points2, expected2);

    testConvexHull(gs, points3, expected3);
    testConvexHull(mc, points3, expected3);

    testConvexHull(gs, points4, expected4);
    testConvexHull(mc, points4, expected4);

    testConvexHull(gs, points5, expected5);
    testConvexHull(mc, points5, expected5);

    delete gs;
    delete mc;

    cout << "All tests passed!" << endl;
    return 0;
}