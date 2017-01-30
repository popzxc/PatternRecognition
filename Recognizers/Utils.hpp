#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

struct Class {
public:
    Class();
    Class(size_t _val);

    static const Class A;
    static const Class B;
    static const size_t CLASSES_NUMBER;

    operator size_t() const {
        return val;
    }

private:
    size_t val;
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
    friend bool operator==(const Point &lhs, const Point &rhs) {
        return abs(lhs.x - rhs.x) < delta && abs(lhs.y - rhs.y) < delta;
    }

    friend Point operator+(Point lhs, const Point& rhs)
    {
        lhs += rhs;
        return lhs;
    }
    template<typename T>
    const Point &operator /=(T number)
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
    static double delta;
};

std::ostream &operator <<(std::ostream &os, const Point &p);

double distance(Point a, Point b);

using PointClass = std::pair<Point, Class>;

#endif // UTILS_HPP
