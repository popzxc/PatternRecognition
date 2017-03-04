#ifndef KMEANS_HPP
#define KMEANS_HPP

#include <map>
#include "Recognizer.hpp"

class KMeans : public Recognizer
{
public:
    enum class StartPointsMethod {
        DATASET_BEGIN,
        RANDOM,
        USER_DEFINED
    };

    KMeans(size_t _nClusters, StartPointsMethod _method = StartPointsMethod::DATASET_BEGIN);
    virtual ~KMeans() = default;

    void train(const std::vector<PointClass> &trainingSet) override;

    Class recognize(Point point) const override;

    std::string name() const override
    {
        return "K-means";
    }
    std::string shortName() const override
    {
        return "KM";
    }

    void setAdditionalInfo(std::ostream &ostr) override;

    void setUserPoints(const std::vector<Point> &points) {
        for (size_t i = 0; i < nClusters; ++i) {
            clusterCenters.push_back(std::make_pair(points[i], Class(i)));
        }
    }

private:
    bool doKMeans(const std::vector<PointClass> &trainingSet);
    double getClusterRadius(const std::vector<Point> &cluster, Point clusterCenter) const;

    size_t nClusters;
    std::vector<PointClass> clusterCenters;
    std::vector<double> clusterSizes;
    std::vector<PointClass> veryBeginning;
    std::map<Class, std::vector<Point> > clusters;
    StartPointsMethod method;
    size_t nSteps;
    template<class T>
    double classSize(const std::vector<T> &vec) const {
        return vec.size() > 0 ? vec.size() : 1;
    }
};

#endif // KMEANS_HPP
