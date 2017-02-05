#ifndef PCA_HPP
#define PCA_HPP

#include "Reductors/Reductor.hpp"

class PCA : public Reductor
{
public:
    PCA();
    virtual ~PCA() = default;

    Axis reduce(std::vector<PointClass> &data, Axis axis = Axis::AUTO) override;
};

#endif // PCA_HPP
