#ifndef REDUCTOR_HPP
#define REDUCTOR_HPP

#include <vector>
#include "Recognizers/Utils.hpp"

class Reductor {
public:
    virtual ~Reductor() = default;

    virtual std::vector<Point> reduce(const std::vector<Point> &data) = 0;
};

#endif // REDUCTOR_HPP
