#include <Eigen/Eigenvalues>
#include "PCA.hpp"

using namespace Eigen;
using namespace std;

PCA::PCA()
{
}

Reductor::Axis PCA::reduce(vector<PointClass> &data, Axis axis)
{
    // Load data to two vectors
    RowVectorXd x(data.size());
    RowVectorXd y(data.size());
    for (vector<Point>::size_type i = 0; i < data.size(); ++i) {
        x[i] = data[i].first.x;
        y[i] = data[i].first.y;
    }

    // Get centered and transformed according to max dispersion directions dataset (with covariance matrix)
    auto processed = whiten(x, y);

    // If user didn't select axis, detect one with the max dispersion value
    if (axis == Axis::AUTO) {
        axis = processed.covarianceMatrix(0, 0) >= processed.covarianceMatrix(1, 1) ? Axis::X : Axis::Y;
    }

    // Lambda to project data on chosen axis
    auto fillPointFromWhitened = [&axis, &processed](auto i) {
        return (axis == Axis::X) ? Point(processed.whitened(0, i), 0) : Point(0, processed.whitened(1, i));
    };

    // Fill the vector with output values
    for (vector<PointClass>::size_type i = 0; i < data.size(); ++i) {
        data[i].first = fillPointFromWhitened(i);
    }

    // Return choosen axis
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

PCA::WhitenedData PCA::whiten(const RowVectorXd &x, const RowVectorXd &y)
{
    WhitenedData ret;

    // Center both x and y
    RowVectorXd xCentered = x.array() - x.mean();
    RowVectorXd yCentered = y.array() - y.mean();

    // Get covariance matrix
    ret.covarianceMatrix = getCovarianceMatrix(xCentered, yCentered);

    // Find eigenvectors ang eigenvalues
    EigenSolver<Matrix2d> es(ret.covarianceMatrix);

    // Resulting matrix
    MatrixXd res(2, x.size());
    res.row(0) = xCentered;
    res.row(1) = yCentered;

    // New axes
    auto complexEigenVector = es.eigenvectors().col(0); // Get first eigenvector
    RowVector2d eigenVectors[2];
    eigenVectors[0] << complexEigenVector[0].real(), complexEigenVector[1].real();
    complexEigenVector = es.eigenvectors().col(1); // Get second eigenvector
    eigenVectors[1] << complexEigenVector[0].real(), complexEigenVector[1].real();

    // Project centered data on new axes
    ret.whitened.resize(2, xCentered.cols());
    ret.whitened << eigenVectors[0] * res, eigenVectors[1] * res;

    return ret;
}
