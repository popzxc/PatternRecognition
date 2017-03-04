#include <cfloat>
#include <map>
#include "ThresholdBasedRecognizer.hpp"

using namespace std;

ThresholdBasedRecognizer::ThresholdBasedRecognizer(double _threshold) : threshold(_threshold)
{
}

void ThresholdBasedRecognizer::train(const vector<PointClass> &trainingSet)
{
    if (!trainingSet.size()) {
        return;
    }

    map<Class, vector<Point> > clusters;
    size_t nClusters = 0;
    clusters[nClusters++] = { trainingSet[0].first };

    clusterCenters = { trainingSet[0].first };
    for (vector<PointClass>::size_type i = 1; i < trainingSet.size(); ++i) {
        auto p = trainingSet[i].first;
        bool found = false;
        for (size_t j = 0; j < nClusters; ++j) {
            if (distance(p, clusterCenters[j]) < threshold) {
                // Add new point to cluster
                clusters[j].push_back(p);
                // Calculate new center
                clusterCenters[j] = getClusterCenter(clusters[j]);
                // Process next point
                found = true;
                break;
            }
        }
        if (!found) {
            // Point wasn't classified, create new class
            clusters[nClusters++] = { p };
            clusterCenters.push_back(p);
        }
    }
}

Class ThresholdBasedRecognizer::recognize(Point point) const
{
    double minDistance = DBL_MAX;
    Class retClass;
    for (vector<Point>::size_type i = 0; i < clusterCenters.size(); ++i) {
        double curDistance = distance(point, clusterCenters[i]);
        if (curDistance < minDistance) {
            retClass = i;
            minDistance = curDistance;
        }
    }
    return retClass;
}

void ThresholdBasedRecognizer::setAdditionalInfo(ostream &ostr)
{
    ostr << shortName() << " " << clusterCenters.size() << " 0 ";
    for (vector<Point>::size_type i = 0; i < clusterCenters.size(); ++i) {
        ostr << clusterCenters[i] << " " << threshold;
        if (i != clusterCenters.size() - 1) {
            ostr << " ";
        }
    }
    ostr << endl;
}
