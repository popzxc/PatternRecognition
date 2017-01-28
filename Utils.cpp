#include <cmath>
#include "Utils.hpp"

std::ostream &operator <<(std::ostream &os, const Point &p)
{
    os << p.x << " " << p.y;
    return os;
}

double distance(Point a, Point b)
{
    double xDiff = a.x - b.x;
    double yDiff = a.y - b.y;
    return sqrt(pow(xDiff, 2) + pow(yDiff, 2));
}
