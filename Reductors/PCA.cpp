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
    RowVectorXd x(data.size());
    RowVectorXd y(data.size());
    for (vector<Point>::size_type i = 0; i < data.size(); ++i) {
        x[i] = data[i].first.x;
        y[i] = data[i].first.y;
    }

    // Center both x and y
    RowVectorXd xCentered = x.array() - x.mean();
    RowVectorXd yCentered = y.array() - y.mean();

    // Get covariance matrix
    Matrix2d cov = getCovarianceMatrix(xCentered, yCentered);

    // Find eigenvectors ang eigenvalues
    EigenSolver<Matrix2d> es(cov);

    // Find the axis we'll project on (if it wasn't set by user)
    if (axis == Axis::AUTO) {
        auto v1 = abs(es.eigenvalues()[0]);
        auto v2 = abs(es.eigenvalues()[1]);
        bool x = v1 >= v2;
        axis = x ? Axis::X : Axis::Y;
    }


    // Resulting matrix
    MatrixXd res(2, x.size());
    res.row(0) = xCentered;
    res.row(1) = yCentered;

    if (axis == Axis::X)
    {
        auto complexEigenVector = es.eigenvectors().col(0); // Get first eigenvector
        RowVector2d eigenVector;
        eigenVector << complexEigenVector[0].real(), complexEigenVector[1].real();

        res = eigenVector * res;
        for (auto i = 0; i < x.size(); ++i) {
            data[i].first.x = res(0, i);
            data[i].first.y = 0;
        }
    } else {
        auto complexEigenVector = es.eigenvectors().col(1); // Get first eigenvector
        RowVector2d eigenVector;
        eigenVector << complexEigenVector[0].real(), complexEigenVector[1].real();

        res = eigenVector * res;
        for (auto i = 0; i < y.size(); ++i) {
            data[i].first.y = res(0, i);
            data[i].first.x = 0;
        }
    }
    return axis;
}

Matrix2d PCA::getCovarianceMatrix(const RowVectorXd &xCentered, const RowVectorXd &yCentered)
{
    /* At this point expected value of x and y is 0
     * And we have fixed number of dimensions (2)
     * So we can simplify formula of covariance matrix
     * Size of covariance matrix is 2*2, and
     * cov(0, 0) = var(x) = sum((x - mean(x)) ^ 2) / len(x) = sum(x ^ 2) / len(x)
     * cov(1, 0) = cov(0, 1) = E(x, y) = sum(x * transpose(y)) / (len(x) * len(y))
     * cov(1, 1) calculates for y the same way as for x
     */
    Matrix2d cov;
    double disp = 0.0;
    for (auto i = 0; i < xCentered.size(); ++i) {
        disp += xCentered[i] * xCentered[i];
    }
    cov(0, 0) = disp / xCentered.size();
    disp = 0.0;
    for (auto i = 0; i < yCentered.size(); ++i) {
        disp += yCentered[i] * yCentered[i];
    }
    cov(1, 1) = disp / yCentered.size();
    cov(0, 1) = cov(1, 0) = (xCentered * yCentered.transpose()).sum() / (xCentered.size() * yCentered.size());
    return cov;
}
