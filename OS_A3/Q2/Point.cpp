#include "Point.hpp"

Orientation Point::orientation(const Point &mid, const Point &other) const {
    float o = (mid.getY() - getY()) * (other.getX() - getX()) - (mid.getX() - getX()) * (other.getY() - getY());
    if (o == 0) return COLLINEAR;
    return (o > 0) ? CLOCKWISE : COUNTER_CLOCKWISE;
}