#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include "PCA.hpp"

using namespace Eigen;
using namespace std;

PCA::PCA()
{

}

Reductor::Axis PCA::reduce(std::vector<PointClass> &data, Axis axis)
{
    // Load data to two vectors
    vector<double> xV(data.size());
    vector<double> yV(data.size());
    for (vector<Point>::size_type i = 0; i < data.size(); ++i) {
        xV[i] = data[i].first.x;
        yV[i] = data[i].first.y;
    }

    // Map std vector to eigen vector
    Map<RowVectorXd> x(xV.data(), xV.size());
    Map<RowVectorXd> y(yV.data(), yV.size());

    // Center both x and y
    auto xMean = x.mean();
    auto yMean = y.mean();
    RowVectorXd xCentered = x.array() - xMean;
    RowVectorXd yCentered = y.array() - yMean;


    /* At this point expected value of x and y is 0
     * And we have fixed number of dimensions (2)
     * So we can simplify formula of covariance matrix
     * Size of covariance matrix is 2*2, and
     * cov(0, 0) = var(x) = sum(x - mean(x)) ^ 2 / len(x) = sum(x) ^ 2 / len(x)
     * cov(1, 0) = cov(0, 1) = E(x, y) = sum(x * transpose(y)) / (len(x) * len(y))
     * cov(1, 1) calculates for y the same way as for x
     */
    Matrix2d cov;
    cov(0, 0) = xCentered.sum() * xCentered.sum() / xCentered.size();
    cov(1, 1) = yCentered.sum() * yCentered.sum() / yCentered.size();
    cov(0, 1) = cov(1, 0) = (xCentered * yCentered.transpose()).sum() / (xCentered.size() * yCentered.size());

    MatrixXd res(2, x.size());
    res.row(0) = xCentered;
    res.row(1) = yCentered;

    EigenSolver<Matrix2d> es(cov);
    if (axis == Axis::AUTO) {
        bool x = abs(es.eigenvalues()[0].real()) > abs(es.eigenvalues()[1].real());
        axis = x ? Axis::X : Axis::Y;
    }
    if (axis == Axis::X) {
        // X axis size is greater than Y
        auto complexEigenVector = es.eigenvectors().col(0); // Get first eigenvector
        RowVector2d eigenVector;
        eigenVector << complexEigenVector[0].real(), complexEigenVector[1].real();

        res = eigenVector * res;
        for (auto i = 0; i < x.size(); ++i) {
            data[i].first.x = res(0, i);
            data[i].first.y = 0;
        }
    } else {
        // Y axis size is greater than X
        auto complexEigenVector = es.eigenvectors().col(1); // Get first eigenvector
        RowVector2d eigenVector;
        eigenVector << complexEigenVector[0].real(), complexEigenVector[1].real();

        res = eigenVector * res;
        // Project anyway on X axis
        for (auto i = 0; i < x.size(); ++i) {
            data[i].first.y = res(0, i);
            data[i].first.x = 0;
        }
    }
    return axis; // Not so good, meh
}
