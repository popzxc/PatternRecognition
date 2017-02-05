#include <cmath>
#include "NearestCentroid.hpp"

using std::vector;

NearestCentroid::NearestCentroid()
{
    etalons.resize(Class::CLASSES_NUMBER);
    clusterSizes.resize(etalons.size());
}

void NearestCentroid::train(const vector<PointClass> &trainingSet)
{
    vector<size_t> counts(etalons.size());
    for (auto &entry : trainingSet) {
        etalons[static_cast<size_t>(entry.second)].first += entry.first;
        etalons[static_cast<size_t>(entry.second)].second = entry.second;
        counts[static_cast<size_t>(entry.second)] += 1;
    }
    for (vector<size_t>::size_type i = 0; i < etalons.size(); ++i) {
        if (counts[i] == 0)
            continue;

        // Getting etalon
        etalons[i].first /= counts[i];

        // Getting cluster size
        for (auto &entry : trainingSet) {
            if (etalons[i].second == entry.second) {
                clusterSizes[i] += pow(distance(etalons[i].first, entry.first), 2);
            }
        }
        clusterSizes[i] = sqrt(clusterSizes[i] / counts[i]);
    }
}

Class NearestCentroid::recognize(Point point) const
{
    Class retval = etalons[0].second;
    double minDistance = distance(etalons[0].first, point) / clusterSizes[0]; // FIXME check for 0
    for (vector<size_t>::size_type i = 0; i < etalons.size(); ++i) {
        if (clusterSizes[i] == 0.0)
            continue;
        double currentDistance = distance(etalons[i].first, point) / clusterSizes[i];
        if (currentDistance < minDistance) {
            minDistance = currentDistance;
            retval = etalons[i].second;
        }
    }
    return retval;
}
