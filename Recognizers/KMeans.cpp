#include <vector>
#include <cfloat>
#include <algorithm>
#include <map>
#include "KMeans.hpp"

using namespace std;

KMeans::KMeans(size_t _nClusters, StartPointsMethod _method) : nClusters(_nClusters), method(_method), nSteps(0)
{
}

void KMeans::train(const vector<PointClass> &trainingSet)
{
    // Available only when trainingSet.size() >= nClusters
    for (vector<PointClass>::size_type i = 0; i < nClusters; ++i) {
        clusterSizes.push_back(1);
    }
    switch(method) {
        case StartPointsMethod::DATASET_BEGIN:
            for (vector<PointClass>::size_type i = 0; i < nClusters; ++i) {
                PointClass center = make_pair(trainingSet[i].first, Class(i));
                clusterCenters.push_back(center);
            }
            break;
        case StartPointsMethod::RANDOM:
        {
            vector<size_t> indices;
            for (size_t i = 0; i < nClusters; ++i) {
                size_t num = 0;
                do {
                    num = rand() % trainingSet.size();
                } while(find(indices.begin(), indices.end(), num) != indices.end());
                indices.push_back(num);
                PointClass center = make_pair(trainingSet[num].first, Class(i));
                clusterCenters.push_back(center);
            }
            break;
        }
        case StartPointsMethod::USER_DEFINED:
        {
            // Assuming that setUserPoints() is called already
            break;
        }
    }
    veryBeginning = clusterCenters;

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
            ++nSteps;
        } while (!end);
        cout << nSteps;
    }
}

Class KMeans::recognize(Point point) const
{
    double minDistance = DBL_MAX;
    Class retClass;
    for (vector<PointClass>::size_type i = 0; i < clusterCenters.size(); ++i) {
        double curDistance = distance(point, clusterCenters[i].first) / clusterSizes[i];
//        double curDistance = distance(point, clusterCenters[i].first);
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
        Point curPoint = el.first;
        Class curClass = recognize(curPoint);
        clusters[curClass].push_back(curPoint);
    }

    // Find new cluster centers
    vector<PointClass> newClusterCenters;
    vector<double> newClusterSizes;
    for (auto &el : clusters) {
        Point clusterCenter = getClusterCenter(el.second);
        double clusterRadius = getClusterRadius(el.second, clusterCenter);
        newClusterCenters.push_back(make_pair(clusterCenter, el.first));
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

double KMeans::getClusterRadius(const std::vector<Point> &cluster, Point clusterCenter) const
{
    double clusterRadius = 0.0;
    for (auto &entry : cluster) {
        double dist = distance(entry, clusterCenter);
        clusterRadius += dist * dist;
    }
    clusterRadius = sqrt(clusterRadius / classSize(cluster));
    return clusterRadius;
}

void KMeans::setAdditionalInfo(ostream &ostr)
{
    ostr << shortName() << " " << nClusters << " " << nSteps << " ";
    for (vector<PointClass>::size_type i = 0; i < veryBeginning.size(); ++i) {
        ostr << veryBeginning[i].first << " " << clusterSizes[i];
        if (i != veryBeginning.size() - 1) {
            ostr << " ";
        }
    }
    ostr << endl;
}
