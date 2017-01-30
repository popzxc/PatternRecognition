#include <cmath>
#include "Utils.hpp"

const Class Class::A = Class(0);
const Class Class::B = Class(1);
const size_t Class::CLASSES_NUMBER = 2;

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

Class::Class() : val(0)
{
}

Class::Class(size_t _val) : val(_val)
{
}
