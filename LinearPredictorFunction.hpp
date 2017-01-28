#ifndef LINEARPREDICTORFUNCTION_HPP
#define LINEARPREDICTORFUNCTION_HPP

#include <functional>
#include <Eigen/Core>
#include "Recognizer.hpp"

double legendrePolynomial(int n, double x);

class LinearPredictorFunction : public Recognizer {
public:
    LinearPredictorFunction(int _nFeatures, std::function<double(int, double)> _phi = legendrePolynomial);

    virtual ~LinearPredictorFunction() = default;

    void train(const std::vector<PointClass> &trainingSet) override;

    Class recognize(Point point) const override;

    std::string name() const override
    {
        return "Decision function method";
    }

private:
    int nFeatures;
    Eigen::VectorXd coefs;
    std::function<double(int, double)> phi;
    std::vector<std::pair<int, int> > pows;

    Eigen::MatrixXd extend(const std::vector<Point> &points);
    void fillCoeffs(Point p, std::vector<double> &coefs) const;

};

#endif // LINEARPREDICTORFUNCTION_HPP
