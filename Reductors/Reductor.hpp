#ifndef REDUCTOR_HPP
#define REDUCTOR_HPP

#include <vector>
#include "Recognizers/Utils.hpp"

class Reductor {
public:
    enum class Axis {
        AUTO,
        X,
        Y
    };

    virtual ~Reductor() = default;

    virtual Axis reduce(std::vector<PointClass> &data, Axis axis = Axis::AUTO) = 0;
};

#endif // REDUCTOR_HPP
