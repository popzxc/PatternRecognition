#include <cmath>
#include <map>
#include "LabManager.hpp"

using namespace std;

LabManager::LabManager()
{

}

void LabManager::executeLab(int argc, char *argv[])
{
    args.clear();
    int i = 0;
    const char *curArg;
    while (++i < argc) {
        if (argv[i][0] == '-') {
            args[argv[i]] = vector<string>();
            curArg = argv[i];
        } else {
            args[curArg].push_back(argv[i]);
        }
    }

    // Debug
    for (auto it = args.begin(); it != args.end(); ++it) {
        cout << it->first << ": ";
        for (auto& arg : it->second) {
            cout << arg << "; ";
        }
        cout << endl;
    }
}

void LabManager::executeLab1()
{

}

void LabManager::executeLab2()
{
    //     auto simpleF = [](int p, double x) -> double { return pow(x, p); };
}

void LabManager::executeLab3()
{

}

void LabManager::executeLab4()
{

}

vector<PointClass> LabManager::readFile(string name)
{
    vector<PointClass> points;
    ifstream input(name);
    if (!input.is_open()) {
        return points;
    }
    double x, y;
    size_t mark;
    while (!input.eof()) {
        input >> x >> y >> mark;
        points.push_back(make_pair(Point{x, y}, static_cast<Class>(mark)));
    }
    return points;
}

void LabManager::runCheck(Recognizer &r, vector<PointClass> &trSet,
              vector<PointClass> &points, ostream *out)
{
    r.train(trSet);
    for (auto &point : points) {
        auto real = point.second;
        auto guess = r.recognize(point.first);
        bool correct = real == guess;
        if (out) {
            *out << point.first << " " << static_cast<size_t>(point.second) << " " << correct << endl;
        }
    }
}

void LabManager::defaultCheck(Recognizer &r)
{
    vector<PointClass> exampleTrainSet = {
        make_pair(Point{10.0, 10.0}, Class::A),
        make_pair(Point{11.1, 10.2}, Class::A),
        make_pair(Point{10.0, 10.4}, Class::A),
        make_pair(Point{10.3, 10.8}, Class::A),
        make_pair(Point{10.4, 10.5}, Class::A),
        make_pair(Point{0.0, 0.0}, Class::B),
        make_pair(Point{0.0, 0.2}, Class::B),
        make_pair(Point{0.3, 0.1}, Class::B),
        make_pair(Point{0.0, 0.4}, Class::B),
        make_pair(Point{0.4, 0.3}, Class::B)
    };

    vector<PointClass> exampleCheckSet = {
        make_pair(Point{9.0, 9.4}, Class::A),
        make_pair(Point{10.2, 10.4}, Class::A),
        make_pair(Point{11.2, 12.4}, Class::A),
        make_pair(Point{9.2, 12.4}, Class::A),
        make_pair(Point{10.0, 10.0}, Class::A),
        make_pair(Point{0.1, 0.0}, Class::B),
        make_pair(Point{-0.3, 1.0}, Class::B),
        make_pair(Point{1.3, 1.0}, Class::B),
        make_pair(Point{0.3, 0.0}, Class::B),
        make_pair(Point{4.3, 0.0}, Class::B)
    };
    runCheck(r, exampleTrainSet, exampleCheckSet, &cout);
}

void LabManager::mainCheck(Recognizer &r, string trainFile, string checkFile, string outputFile)
{
    auto trainingSet = readFile(trainFile);
    auto points = readFile(checkFile);
    ofstream output(outputFile);
    runCheck(r, trainingSet, points, &output);
}
