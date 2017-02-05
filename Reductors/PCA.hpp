#ifndef PCA_HPP
#define PCA_HPP

#include <Eigen/Core>
#include "Reductors/Reductor.hpp"

class PCA : public Reductor
{
public:
    PCA();
    virtual ~PCA() = default;

    Axis reduce(std::vector<PointClass> &data, Axis axis = Axis::AUTO) override;

    struct WhitenedData {
        Eigen::MatrixXd whitened;
        Eigen::Matrix2d covarianceMatrix;
    };

    static WhitenedData whiten(const Eigen::RowVectorXd &xCentered, const Eigen::RowVectorXd &y);
    static Eigen::Matrix2d getCovarianceMatrix(const Eigen::RowVectorXd &xCentered, const Eigen::RowVectorXd &yCentered);
};

#endif // PCA_HPP
