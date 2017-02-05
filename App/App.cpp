#include <sstream>
#include <cmath>
#include <fstream>
#include "App.hpp"
#include "Recognizers/LinearPredictorFunction.hpp"
#include "Recognizers/NearestCentroid.hpp"
#include "Recognizers/NearestNeighbour.hpp"
#include "Recognizers/KMeans.hpp"
#include "Recognizers/ThresholdBasedRecognizer.hpp"
#include "Reductors/FakeReductor.hpp"
#include "Reductors/PCA.hpp"

using namespace std;

App::App(int argc, char *argv[])
{
    auto i = 0;
    const char *curArg;
    while (++i < argc) {
        if (argv[i][0] == '-') {
            args[argv[i]] = {};
            curArg = argv[i];
        } else {
            args[curArg].push_back(argv[i]);
        }
    }
}

int App::exec()
{
    // Process "--help"
    if (args.empty() || args.find("--help") != args.end()) {
        showHelp();
        exit(0);
    }

    // Process "--generate"
    if (args.find("--generate") != args.end()) {
        generateDataset(args["--generate"]);
    }

    auto method = args.find("--method");
    if (method == args.end() || method->second.size() == 0) {
        return 0;
    }

    // FIXME cludge
    if (args.find("--default") != args.end()) {
        runDefault = true;
    } else {
        runDefault = false;
        trFile = args["--input"][0];
        chFile = args["--input"][1];
        output = args["--output"][0];
    }

    string reductor = "";
    if (args.find("--reductor") != args.end()) {
        reductor = args["--reductor"][0];
    }

    executeMethod(method->second, reductor);

    if (args.find("--analyze") != args.end()) {
        // FIXME
        string command = "./analyze.py ";
        command += output;
        system(command.c_str());
    }

    return 0;
}

void App::showHelp()
{
    cout << "Available arguments:\n"
            "--method [methodName [methodArgs]]\n"
            "    [methodName]:\n"
            "        nn - nearest neighbour. No arguments needed\n"
            "        nc - nearest centroid. No arguments needed\n"
            "        lpf - linear predictor function. 1 argument: file to output coefficients of polynomial.\n"
            "        km - k-means. No arguments needed\n"
            "        tb - thredhold based recognizer. No arguments needed\n"
            "        ...to be continued\n"
            "\n"
            "--generate [path_to_script type npoints noise]\n"
            "    [path_to_script] - path to generate_points.py\n"
            "    [type] - type of spread: simple | brackets | circle\n"
            "    [npoints] - number of points in each cluster\n"
            "    [noise] - number from 0 to 1, percent of fake points\n"
            "\n"
            "--default\n"
            "    Run test check\n"
            "\n"
            "--input [trainingSet checkSet]\n"
            "    [trainingSet] - path to file with training set\n"
            "    [checkSet] - path to file with set to check\n"
            "\n"
            "--analyze [methodName [methodArgs]]\n"
            "    Show result of recognition. Argumetns are the same as for --method\n"
            "\n"
            "--help\n"
            "    Show this message"
         << endl;
}

void App::generateDataset(const vector<string> &args)
{
    ostringstream command;
    for (auto &arg : args) {
        command << arg << " ";
    }
    system(command.str().c_str());
}

void App::executeMethod(const vector<string> &args, const string &reductorType)
{
    const string &method = args[0];

    Recognizer *recognizer = nullptr;
    if (method == "nn") {
        recognizer = new NearestNeighbour();
    } else if (method == "nc") {
        recognizer = new NearestCentroid();
    } else if (method == "lpf") {
        recognizer = new LinearPredictorFunction(10); // FIXME
    } else if (method == "km") {
        recognizer = new KMeans(2); // FIXME
    } else if (method == "tb") {
        recognizer = new ThresholdBasedRecognizer(5.0); // FIXME
    }

    Reductor *reductor = nullptr;
    if (reductorType == "PCA") {
        reductor = new PCA;
    } else {
        reductor = new FakeReductor;
    }

    if (runDefault) {
        defaultCheck(*recognizer);
    } else {
        mainCheck(*recognizer, *reductor, trFile, chFile, output);
    }

    delete reductor;
    delete recognizer;
}

vector<PointClass> App::readFile(string name)
{
    vector<PointClass> points;
    ifstream input(name);
    if (!input.is_open()) {
        return points;
    }
    double x, y;
    size_t mark;
    // Assuming that provided file is always correct
    while (!input.eof()) {
        input >> x >> y >> mark;
        points.push_back(make_pair(Point{x, y}, static_cast<Class>(mark)));
    }
    return points;
}

void App::runCheck(Recognizer &r, vector<PointClass> &trSet,
              vector<PointClass> &points, ostream *out)
{
    r.train(trSet);
    for (auto &point : points) {
        auto real = point.second;
        auto guess = r.recognize(point.first);
        bool correct = real == guess;
        if (out) {
            *out << point.first << " " << point.second << " " << correct << endl;
        }
    }
}

void App::defaultCheck(Recognizer &r)
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
    cout << r.name() << endl;
    runCheck(r, exampleTrainSet, exampleCheckSet, &cout);
}

void App::mainCheck(Recognizer &recognizer, Reductor &reductor, string trainFile, string checkFile, string outputFile)
{
    auto trainingSet = readFile(trainFile);
    auto axis = reductor.reduce(trainingSet);
    auto points = readFile(checkFile);
    reductor.reduce(points, axis);
    ofstream output(outputFile);
    runCheck(recognizer, trainingSet, points, &output);
}
