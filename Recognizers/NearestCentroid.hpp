#ifndef NEARESTCENTROID_HPP
#define NEARESTCENTROID_HPP

#include "Recognizer.hpp"

class NearestCentroid : public Recognizer {
public:
    NearestCentroid();

    virtual ~NearestCentroid() = default;

    void train(const std::vector<PointClass> &trainingSet) override;

    Class recognize(Point point) const override;

    std::string name() const override
    {
        return "Nearest centroid method";
    }
    std::string shortName() const override
    {
        return "NC";
    }

    void setAdditionalInfo(std::ostream &ostr) override;

private:
    std::vector<PointClass> etalons;
    std::vector<double> clusterSizes;
};

#endif // NEARESTCENTROID_HPP
