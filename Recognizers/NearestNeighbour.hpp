#ifndef NEARESTNEIGHBOUR_HPP
#define NEARESTNEIGHBOUR_HPP

#include "Recognizer.hpp"

class NearestNeighbour : public Recognizer {
public:
    virtual ~NearestNeighbour() = default;

    void train(const std::vector<PointClass> &trainingSet) override;

    Class recognize(Point point) const override;

    std::string name() const override
    {
        return "Nearest neighbour method";
    }

    std::string shortName() const override
    {
        return "NN";
    }

    void setAdditionalInfo(std::ostream &ostr) override;

private:
    std::vector<PointClass> set;
};

#endif // NEARESTNEIGHBOUR_HPP
