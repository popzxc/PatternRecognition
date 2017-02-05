#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include "Reductors/PCA.hpp"
#include "ICA.hpp"

using namespace std;
using namespace Eigen;

ICA::ICA()
{

}

Reductor::Axis ICA::reduce(vector<PointClass> &data, Axis axis)
{
    // Function, its first and second derivative for FastICA algorithm
    auto f  = [](auto matrix) { return matrix.array().cosh().log(); };
    auto g  = [](auto matrix) { return matrix.array().tanh(); };
    auto g_ = [](auto matrix) { return 1 - matrix.array().tanh().square(); };

    // Load data to two vectors
    RowVectorXd x(data.size());
    RowVectorXd y(data.size());
    for (vector<Point>::size_type i = 0; i < data.size(); ++i) {
        x[i] = data[i].first.x;
        y[i] = data[i].first.y;
    }

    // Get whitened data
    auto processed = PCA::whiten(x, y);

    return axis;
}
