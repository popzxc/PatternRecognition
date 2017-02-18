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
    // FIXME average every column!
    auto E  = [](auto matrix) {
        return static_cast<MatrixXd>((matrix.row(0) + matrix.row(1) / 2));
    }; // TODO

    // Load data to two vectors
    RowVectorXd x(data.size());
    RowVectorXd y(data.size());
    for (vector<Point>::size_type i = 0; i < data.size(); ++i) {
        x[i] = data[i].first.x;
        y[i] = data[i].first.y;
    }

    // Get whitened data
    auto processed = PCA::whiten(x, y);
    auto X = processed.whitened; // Rows: x, y.

//    MatrixXd w = MatrixXd::Random(2);

//    auto M = data.size();
//    for (int i = 0; i < 2; ++i) {
//        MatrixXd w_i = RowVectorXd::Random(data.size()).transpose(); // Column vector
//        MatrixXd lastW;
//        do {
//            lastW = w_i;

////            w_i = (1.0 / M) * X * g(w_i.transpose() * X).transpose() - (1.0 / M) * g_(w_i.transpose() * X).col(0) * w_i;
//            auto debug1 = X * g(w_i.transpose() * X).transpose();
//            MatrixXd debug2 = g_(w_i.transpose() * X).col(0);
//            MatrixXd debug3 = debug2 * w_i.transpose();
//            w_i = (1.0 / M) * debug1 - (1.0 / M) * debug3;
//            // DO THINK
//            w_i = w_i / w.size(); // FIXME
//        } while((w_i.transpose() * lastW).sum() < 0.9); // FIXME
//    }

    MatrixXd w = RowVectorXd::Random(2).transpose(); // Column vector
    decltype(w) lastW;
    do {
        lastW = w;
//        w = E(X * g(wT * X)) - E(g_(wT * X)) * w;
        MatrixXd wT = w.transpose();
        MatrixXd m1 = wT * X;
        MatrixXd debug1 = g(m1).transpose();
        MatrixXd debug2 = X * debug1;
        MatrixXd E1 = debug2;
        MatrixXd debug3 = g_(wT * X);
        //auto debug4 = E(debug3);
        auto debug4 = debug3.mean();
        MatrixXd E2 = debug4 * w;
        w = E1 - E2;
        w /= w.size();
    } while ((w.transpose() * lastW).sum() < 0.9); // FIXME

//    for (vector<PointClass>::size_type i = 0; i < data.size(); ++i) {
//          data[i].first.x =
//    }

    return axis;
}
