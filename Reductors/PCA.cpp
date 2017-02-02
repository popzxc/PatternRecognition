#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include "PCA.hpp"

using namespace Eigen;
using namespace std;

PCA::PCA()
{

}

vector<Point> PCA::reduce(const vector<Point> &data)
{
    // Load data to two vectors
    vector<double> xV(data.size());
    vector<double> yV(data.size());
    for (vector<Point>::size_type i = 0; i < data.size(); ++i) {
        xV[i] = data[i].x;
        yV[i] = data[i].y;
    }

    // Map std vector to eigen vector
    Map<RowVectorXd> x(xV.data(), xV.size());
    Map<RowVectorXd> y(yV.data(), yV.size());

    // Center both x and y
    RowVectorXd xCentered = x.array() - x.mean();
    RowVectorXd yCentered = y.array() - y.mean();


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

    EigenSolver<Matrix2d> es(cov);

    // Compare new axis sizes
    if (es.eigenvalues()[0].real() > es.eigenvalues()[1].real()) {
        // X axis size is greater than Y
        auto complexEigenVector = es.eigenvectors()[0];
        RowVector2d eigenVector;
        eigenVector << complexEigenVector.real(), complexEigenVector.imag();
        RowVector2d xRes = xCentered.dot(eigenVector);
        y.fill(0);
    } else {
        // Y axis size is greater than X
//        y = yCentered.dot(es.eigenvectors()[1]);
        y.fill(0);
    }

    return data; // Not so good, meh
}
