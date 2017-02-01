#ifndef KMEANS_HPP
#define KMEANS_HPP

#include <map>
#include "Recognizer.hpp"

class KMeans : public Recognizer
{
public:
    KMeans(size_t _nClusters);
    virtual ~KMeans() = default;

    void train(const std::vector<PointClass> &trainingSet) override;

    Class recognize(Point point) const override;

    std::string name() const override
    {
        return "K-means";
    }
private:
    bool doKMeans(const std::vector<PointClass> &trainingSet);

    size_t nClusters;
    std::vector<PointClass> clusterCenters;
    std::vector<size_t> clusterSizes;
    std::map<Class, std::vector<Point> > clusters;
};

#endif // KMEANS_HPP
