#ifndef ICA_HPP
#define ICA_HPP

#include "Reductors/FakeReductor.hpp"

class ICA : public Reductor
{
public:
    ICA();
    virtual ~ICA() = default;

    Axis reduce(std::vector<PointClass> &data, Axis axis = Axis::AUTO) override;
};

#endif // ICA_HPP
