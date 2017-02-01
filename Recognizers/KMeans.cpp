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
        PointClass center = make_pair(trainingSet[i].first, Class(i));
        clusterCenters.push_back(center);
        clusterSizes.push_back(1);
    }

    if (trainingSet.size() > nClusters) {
        // Create empty vectors for every class
        for (auto el : trainingSet) {
            if (clusters.find(el.second) == clusters.end()) {
                clusters[el.second] = {};
            }
        }
        bool end;
        do {
            end = doKMeans(trainingSet);
        } while (!end);
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

    // Find new cluster centers
    vector<PointClass> newClusterCenters;
    vector<size_t> newClusterSizes;
    for (auto& el : clusters) {
        // Calculate new cluster center
        Point clusterCenter = getClusterCenter(el.second);
        newClusterCenters.push_back(make_pair(clusterCenter, el.first));

        // Calculate cluster radius
        double clusterRadius = 0;
        for (auto &entry : el.second) {
            double sqrtDist = distance(entry, clusterCenter);
            clusterRadius += sqrtDist * sqrtDist;
        }
        clusterRadius = sqrt(clusterRadius / (el.second.size() + 1));
        newClusterSizes.push_back(clusterRadius);
    }

    // If cluster centers are the same, then the algorithm is finished
    bool end = is_permutation(newClusterCenters.begin(), newClusterCenters.end(), clusterCenters.begin());
    if (!end) {
        clusterCenters = std::move(newClusterCenters);
        clusterSizes = std::move(newClusterSizes);
    }
    return end;
}
