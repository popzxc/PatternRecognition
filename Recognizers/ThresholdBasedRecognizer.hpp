#ifndef THRESHOLDBASEDRECOGNIZER_HPP
#define THRESHOLDBASEDRECOGNIZER_HPP

#include "Recognizer.hpp"

class ThresholdBasedRecognizer : public Recognizer
{
public:
    ThresholdBasedRecognizer(double _threshold);
    virtual ~ThresholdBasedRecognizer() = default;

    void train(const std::vector<PointClass> &trainingSet) override;

    Class recognize(Point point) const override;

    std::string name() const override
    {
        return "Threshold based recognizer";
    }
private:
    double threshold;
    std::vector<Point> clusterCenters;
};

#endif // THRESHOLDBASEDRECOGNIZER_HPP
