#include "NearestNeighbour.hpp"

using std::vector;

void NearestNeighbour::train(const vector<PointClass> &trainingSet)
{
    set = trainingSet;
}

Class NearestNeighbour::recognize(Point point) const
{
    if (set.empty()) {
        return Class::A;
    }
    Class retval = set[0].second;
    double minDistance = distance(set[0].first, point);
    for (vector<PointClass>::size_type i = 1; i < set.size(); ++i) {
        double currentDistance = distance(set[i].first, point);
        if (currentDistance < minDistance) {
            minDistance = currentDistance;
            retval = set[i].second;
        }
    }
    return retval;
}
