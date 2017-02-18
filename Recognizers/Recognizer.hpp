#ifndef RECOGNIZER_HPP
#define RECOGNIZER_HPP

#include <vector>
#include <string>
#include <ostream>
#include "Utils.hpp"

class Recognizer {
public:
    virtual ~Recognizer()
    {
    }
    virtual void train(const std::vector<PointClass> &trainingSet) = 0;
    virtual Class recognize(Point point) const = 0;
    virtual std::string name() const = 0;
    virtual std::string shortName() const = 0;
    virtual void setAdditionalInfo(std::ostream &/*ostr*/)
    {
    }
};

#endif // RECOGNIZER_HPP
