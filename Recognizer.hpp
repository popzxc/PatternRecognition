#ifndef RECOGNIZER_HPP
#define RECOGNIZER_HPP

#include <vector>
#include <string>
#include "Utils.hpp"

class Recognizer {
public:
    virtual ~Recognizer()
    {
    }
    virtual void train(const std::vector<PointClass> &trainingSet) = 0;
    virtual Class recognize(Point point) const = 0;
    virtual std::string name() const = 0;
};

#endif // RECOGNIZER_HPP
