#include "FakeReductor.hpp"

FakeReductor::FakeReductor()
{

}

Reductor::Axis FakeReductor::reduce(std::vector<PointClass> &/*data*/, Axis axis)
{
    return axis;
}
