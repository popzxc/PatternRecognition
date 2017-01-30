#include <vector>
#include <cfloat>
#include <algorithm>
#include <map>
#include "KMeans.hpp"

using namespace std;

KMeans::KMeans(size_t _nClusters) : nClusters(_nClusters)
{
}

void KMeans::train(const vector<PointClass> &trainingSet)
{
    for (vector<PointClass>::size_type i = 0; i < min(nClusters, trainingSet.size()); ++i) {
        clusterCenters.push_back(trainingSet[i]);
        clusterSizes.push_back(1);
    }

    if (trainingSet.size() > nClusters) {
        // Create empty vectors for every class
        for (auto el : trainingSet) {
            if (clusters.find(el.second) == clusters.end()) {
                clusters[el.second] = {};
            }
        }
        while(doKMeans(trainingSet));
    }
}

Class KMeans::recognize(Point point) const
{
    double minDistance = DBL_MAX;
    Class retClass;
    for (vector<PointClass>::size_type i = 0; i < clusterCenters.size(); ++i) {
        double curDistance = distance(point, clusterCenters[i].first);
        if (curDistance < minDistance) {
            retClass = clusterCenters[i].second;
            minDistance = curDistance;
        }
    }
    return retClass;
}

bool KMeans::doKMeans(const vector<PointClass> &trainingSet)
{
    // Clear all clusters
    for (auto& el : clusters) {
        el.second.clear();
    }

    // Fill clusters again (with new cluster centers)
    for (auto& el : trainingSet) {
        Class curClass;
        Point curPoint = el.first;
        double minDistance = DBL_MAX;
        for (vector<PointClass>::size_type i = 0; i < clusterCenters.size(); ++i) {
            auto curDistance = abs(distance(curPoint, clusterCenters[i].first)) / (clusterSizes[i] + 1);
            if (curDistance < minDistance) {
                minDistance = curDistance;
                curClass = clusterCenters[i].second;
            }
        }
        clusters[curClass].push_back(curPoint);
    }

    // Calculate new cluster centers
    vector<PointClass> newClusterCenters;
    vector<size_t> newClusterSizes;
    for (auto& el : clusters) {
        newClusterCenters.push_back(make_pair(getClusterCenter(el.second), el.first));
        newClusterSizes.push_back(el.second.size());
    }

    // If cluster centers are the same, then the algorithm is finished
    bool end = is_permutation(newClusterCenters.begin(), newClusterCenters.end(), clusterCenters.begin());
    if (!end) {
        clusterCenters = std::move(newClusterCenters);
        clusterSizes = std::move(newClusterSizes);
    }
    return end;
}

Point KMeans::getClusterCenter(const std::vector<Point> &cluster)
{
    Point center;
    for (auto& p : cluster) {
        center += p;
    }
    center /= cluster.size();
    return center;
}
