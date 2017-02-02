#ifndef PCA_HPP
#define PCA_HPP

#include "Reductors/Reductor.hpp"

class PCA : public Reductor
{
public:
    PCA();
    virtual ~PCA() = default;

    std::vector<Point> reduce(const std::vector<Point> &data) override;
};

#endif // PCA_HPP
