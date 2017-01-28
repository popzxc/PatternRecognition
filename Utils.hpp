#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

enum class Class : size_t {
    A = 0,
    B,
    CLASSES_NUMBER
};

struct Point {
    Point() : x(0), y(0)
    {
    }
    Point(double _x, double _y) : x(_x), y(_y)
    {
    }
    Point &operator +=(const Point &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    friend Point operator+(Point lhs, const Point& rhs)
    {
        lhs += rhs;
        return lhs;
    }
    template<typename T>
    Point &operator /=(T number)
    {
        x /= number;
        y /= number;
        return *this;
    }
    template<typename T>
    friend Point operator/(Point lhs, T number)
    {
        lhs /= number;
        return lhs;
    }
    double x;
    double y;
};

std::ostream &operator <<(std::ostream &os, const Point &p);

double distance(Point a, Point b);

using PointClass = std::pair<Point, Class>;

#endif // UTILS_HPP
