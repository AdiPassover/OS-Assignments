#include <cmath>

#ifndef OS_A3_POINT_HPP
#define OS_A3_POINT_HPP

enum Orientation {
    COLLINEAR = 0,
    CLOCKWISE = -1,
    COUNTER_CLOCKWISE = 1
};

class Point {
private:
    float x;
    float y;

public:
    Point() : x(0), y(0) {}
    Point(float x, float y) : x(x), y(y) {}

    float getX() const { return x; }
    float getY() const { return y; }

    Orientation orientation(const Point& mid, const Point& other) const;

    double distance(const Point& other) const { return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2)); }

    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Point& other) const { return !(*this == other); }

    bool operator<(const Point &p) const { return x < p.x || (x == p.x && y < p.y); }
    bool operator>(const Point &p) const { return p < *this; }
    bool operator<=(const Point &p) const { return !(*this > p); }
    bool operator>=(const Point &p) const { return !(*this < p); }

};


#endif //OS_A3_POINT_HPP
