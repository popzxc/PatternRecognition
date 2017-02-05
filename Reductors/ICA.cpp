#include <Eigen/Core>
#include <Eigen/Dense>
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
//    auto f  = [](auto matrix) { return static_cast<MatrixXd>(matrix.array().cosh().log()); };
    auto g  = [](auto matrix) { return static_cast<MatrixXd>(matrix.array().tanh()); };
    auto g_ = [](auto matrix) { return static_cast<MatrixXd>(1 - matrix.array().tanh().square()); };
    auto E  = [](auto matrix) { return static_cast<MatrixXd>((matrix.col(0) + matrix.col(1)) / 2); }; // TODO

    // Load data to two vectors
    RowVectorXd x(data.size());
    RowVectorXd y(data.size());
    for (vector<Point>::size_type i = 0; i < data.size(); ++i) {
        x[i] = data[i].first.x;
        y[i] = data[i].first.y;
    }

    // Get whitened data
    auto processed = PCA::whiten(x, y);
    auto X = processed.whitened.transpose(); // Columns: x, y. Rows: training set

    MatrixXd w = RowVectorXd::Random(data.size()).transpose(); // Column vector
    decltype(w) lastW;
    do {
        lastW = w;
//        w = E(X * g(wT * X)) - E(g_(wT * X)) * w;
        auto wT = w.transpose();
        auto m1 = wT * X;
        auto debug1 = g(m1).transpose();
        auto debug2 = X * debug1;
        auto E1 = E(debug2);
        auto debug3 = g_(wT * X);
        auto E2 = E(debug3) * w;
        w = E1 - E2;
        w /= w.size();
    } while (w != lastW);

//    for (vector<PointClass>::size_type i = 0; i < data.size(); ++i) {
//          data[i].first.x =
//    }

    return axis;
}
