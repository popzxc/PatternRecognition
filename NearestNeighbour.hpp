#ifndef NEARESTNEIGHBOUR_HPP
#define NEARESTNEIGHBOUR_HPP

#include "Recognizer.hpp"

class NearestNeighbour : public Recognizer {
public:
    NearestNeighbour(int _nFeatures) : nFeatures(_nFeatures)
    {
    }
    virtual ~NearestNeighbour() = default;

    void train(const std::vector<PointClass> &trainingSet) override;

    Class recognize(Point point) const override;

    std::string name() const override
    {
        return "Nearest neighbour method";
    }

private:
    int nFeatures;
    std::vector<PointClass> set;
};

#endif // NEARESTNEIGHBOUR_HPP
