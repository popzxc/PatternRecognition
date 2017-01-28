#include <Eigen/Core>
#include <Eigen/Dense>
#include "LinearPredictorFunction.hpp"

using namespace std;
using namespace Eigen;

double legendrePolynomial(int n, double x)
{
    if (n == 0) {
        return static_cast<double>(1);
    } else if (n == 1) {
        return x;
    } else {
        n -= 1; // P_{n+1} = (2n+1)/(n+1) * x * P_{n}(x) - n/(n+1) * P_{n-1}(x)
        double res1 = static_cast<double>(2 * n + 1) / (n + 1) * x * legendrePolynomial(n, x);
        double res2 = static_cast<double>(n) / (n + 1) * legendrePolynomial(n - 1, x);
        return res1 - res2;
    }
}

LinearPredictorFunction::LinearPredictorFunction(int _nFeatures, function<double(int, double)> _phi) :
        nFeatures(_nFeatures + 1), coefs(nFeatures), phi(_phi)
{
    auto isEnd = [this](){
        return static_cast<int>(pows.size()) == nFeatures;
    };
    pows.push_back(make_pair(0, 0));
    int curLvl = 1;
    int curLvlThres = 3;
    int lastLvlThres = 1;
    while (true) {
        pows.emplace_back(make_pair(curLvl, 0));
        if (isEnd())
            break;
        pows.emplace_back(make_pair(0, curLvl));
        if (isEnd())
            break;
        int lastEl = pows.size() - 2; // 2 is for
        for (int i = lastEl - lastLvlThres; i < lastEl; ++i) {
            auto curPair = pows[i];
            curPair.first += 1;
            curPair.second += 1;
            pows.emplace_back(curPair);
            if (isEnd()) {
                break;
            }
        }
        if (isEnd())
            break;
        lastLvlThres = curLvlThres;
        curLvlThres += 2;
        curLvl += 1;
    }
    pows.erase(pows.begin());
    pows.push_back(make_pair(0, 0)); // '1' should be last
}

void LinearPredictorFunction::train(const vector<PointClass> &trainingSet)
{
    MatrixXd setCoefs(trainingSet.size(), nFeatures); // Already inited to zeroes
    VectorXd setMarks(trainingSet.size());
    vector<double> rowCoefs(nFeatures);
    for (vector<Point>::size_type i = 0; i < trainingSet.size(); ++i) {
        // Calculate all coefficients (including additional ones) and put them into x matrix
        fillCoeffs(trainingSet[i].first, rowCoefs);
        setCoefs.row(i) = Map<RowVectorXd>(rowCoefs.data(), nFeatures);
        // Fill z matrix. Value is 1 for class A and -1 for class B
        setMarks(i) = (trainingSet[i].second == Class::A) ? 1 : -1;
    }
    coefs = setCoefs.colPivHouseholderQr().solve(setMarks);
}

Class LinearPredictorFunction::recognize(Point point) const
{
    vector<double> pointCoefs(nFeatures);
    fillCoeffs(point, pointCoefs);
    double res = coefs.dot(Map<RowVectorXd>(pointCoefs.data(), nFeatures));
    return (res > 0.0) ? Class::A : Class::B;
}

void LinearPredictorFunction::fillCoeffs(Point p, std::vector<double> &coefs) const
{
    for (int i = 0; i < nFeatures; ++i) {
        coefs[i] = phi(p.x, pows[i].first) * phi(p.y, pows[i].second);
    }
}
