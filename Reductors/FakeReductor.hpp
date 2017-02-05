#ifndef FAKEREDUCTOR_HPP
#define FAKEREDUCTOR_HPP

#include "Reductors/Reductor.hpp"

class FakeReductor : public Reductor
{
public:
    FakeReductor();
    virtual ~FakeReductor() = default;

    Axis reduce(std::vector<PointClass> &data, Axis axis = Axis::AUTO) override;
};

#endif // FAKEREDUCTOR_HPP
