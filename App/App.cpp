#include <sstream>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include "App.hpp"
#include "Recognizers/LinearPredictorFunction.hpp"
#include "Recognizers/NearestCentroid.hpp"
#include "Recognizers/NearestNeighbour.hpp"
#include "Recognizers/KMeans.hpp"
#include "Recognizers/ThresholdBasedRecognizer.hpp"
#include "Reductors/FakeReductor.hpp"
#include "Reductors/ICA.hpp"
#include "Reductors/PCA.hpp"

using namespace std;

App::App(int argc, char *argv[]) : runDefault(false), runAuto(false)
{
    auto i = 0;
    const char *curArg;
    while (++i < argc) {
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            args[argv[i]] = {};
            curArg = argv[i];
        } else {
            args[curArg].push_back(argv[i]);
        }
    }
    srand(time(0));
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
    } else if (args.find("--auto") != args.end()) {
        runAuto = true;
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

    executeMethod(reductor);

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
    command << "./generate_points.py ";
    for (auto &arg : args) {
        command << arg << " ";
    }
    system(command.str().c_str());
}

void App::executeMethod(const string &reductorType)
{
    const string &method = args["--method"][0];

    Recognizer *recognizer = nullptr;
    if (method == "nn") {
        recognizer = new NearestNeighbour();
    } else if (method == "nc") {
        recognizer = new NearestCentroid();
    } else if (method == "lpf") {
        auto polySizeStr = args["--polysize"][0];
        istringstream num(polySizeStr);
        int polySize;
        num >> polySize;
        recognizer = new LinearPredictorFunction(polySize);
    } else if (method == "km") {
        auto nclusters = args["--nclusters"][0];
        istringstream ncl(nclusters);
        int num;
        ncl >> num;
        auto startMethodStr = args["--startmethod"][0];
        KMeans::StartPointsMethod startMethod;
        if (startMethodStr == "beg") {
            startMethod = KMeans::StartPointsMethod::DATASET_BEGIN;
        } else if (startMethodStr == "rnd") {
            startMethod = KMeans::StartPointsMethod::RANDOM;
        } else {
            startMethod = KMeans::StartPointsMethod::USER_DEFINED;
        }
        KMeans *kmeans = new KMeans(num, startMethod);
        if (args.find("--startpoints") != args.end()) {
            auto startPointsStr = args["--startpoints"];
            vector<Point> startPoints;
            for (size_t i = 0; i < startPointsStr.size() - 1; ++i) {
                double x = strtod(startPointsStr[i].c_str(), nullptr);
                double y = strtod(startPointsStr[i + 1].c_str(), nullptr);
                startPoints.push_back(Point(x, y));
            }
            kmeans->setUserPoints(startPoints);
        }
        recognizer = kmeans;
    } else if (method == "tb") {
        auto thresStr = args["--thres"][0];
        istringstream num(thresStr);
        double thres;
        num >> thres;
        recognizer = new ThresholdBasedRecognizer(thres);
    }

    Reductor *reductor = nullptr;
    if (reductorType == "PCA") {
        reductor = new PCA;
    } else if (reductorType == "ICA") {
        reductor = new ICA;
    } else {
        reductor = new FakeReductor;
    }

    if (runDefault) {
        defaultCheck(*recognizer);
    } else if (runAuto) {
        autoCheck(*recognizer);
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
    random_shuffle(trSet.begin(), trSet.end());
    r.train(trSet);
    r.setAdditionalInfo(*out);
    for (auto &point : points) {
        auto real = point.second;
        auto guess = r.recognize(point.first);
        bool correct = real == guess;
        if (out) {
//            *out << point.first << " " << point.second << " " << correct << endl;
            *out << point.first << " " << guess << " " << correct << endl;
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

#define RUN_CHECK(CMD1, CMD2, PRMS) \
    for (int i = 0; i < 3; ++i) { \
        params.open("params.txt"); \
        params << PRMS; \
        params.close(); \
        system(CMD1); \
        if (CMD2[0] != '\0') system(CMD2); \
        trainingSet = readFile(trainFile); \
        points = readFile(checkFile); \
        \
        res.open(resName); \
        runCheck(r, trainingSet, points, &res); \
        res.close(); \
        analyzeCommand[analyzeFileLetterNum] = expNum++; \
        system(analyzeCommand.c_str()); \
        if (expNum > '9') { \
            analyzeCommand[analyzeFileLetterNum - 1]++; \
            expNum = '0'; \
        }\
    }

#define TIME_CHECK(NUM) command = "./generate_points.py simple " NUM " 0.0 ~temp_out0.txt ~temp_out1.txt"; \
    system(command.c_str()); \
    trainingSet = readFile(trainFile); \
    points = readFile(checkFile); \
    \
    startTime = clock(); \
    for (int i = 0; i < N_ITERS; ++i) { \
        r.train(trainingSet); \
    } \
    endTime = clock(); \
    trainTime = static_cast<double>(endTime - startTime) * 1000.0 / (N_ITERS * CLOCKS_PER_SEC); \
    \
    startTime = clock(); \
    for (int i = 0; i < N_ITERS; ++i) { \
        r.recognize(trainingSet[0].first); \
    } \
    endTime = clock(); \
    recTime = static_cast<double>(endTime - startTime) * 1000.0 / (N_ITERS * CLOCKS_PER_SEC); \
    timeStats << NUM";" << trainTime << ";" << recTime << endl;

void App::autoCheck(Recognizer &r)
{
    // Preparation
    vector<PointClass> trainingSet, points;
    string trainFile = "~temp_out0.txt";
    string checkFile = "~temp_out1.txt";
    string resName;
    if (args.find("--res") != args.end()) {
        resName= args["--res"][0];
    } else {
        resName = "res.tst";
    }
    string analyzeCommand = "./analyze.py ";
    analyzeCommand += resName + " ";
    analyzeCommand += r.shortName() + resName + "_00.png";
    int analyzeFileLetterNum = analyzeCommand.length() - 5;
    ofstream res, params;
    char expNum = '0';

    /*
     * CHECKING FOR NOISE INFLUENCE
     */

//    RUN_CHECK("./generate_points.py simple 160 0.0 ~temp_out0.txt",
//              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
//              "5.0 0.0\n5.0 5.0\n0.0 10.0\n5.0 5.0\n");

//    RUN_CHECK("./generate_points.py simple 160 0.1 ~temp_out0.txt",
//              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
//              "5.0 0.0\n5.0 5.0\n0.0 10.0\n5.0 5.0\n");

//    RUN_CHECK("./generate_points.py simple 160 0.2 ~temp_out0.txt",
//              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
//              "5.0 0.0\n5.0 5.0\n0.0 10.0\n5.0 5.0\n");

//    RUN_CHECK("./generate_points.py simple 160 0.3 ~temp_out0.txt",
//              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
//              "5.0 0.0\n5.0 5.0\n0.0 10.0\n5.0 5.0\n");

//    RUN_CHECK("./generate_points.py simple 160 0.4 ~temp_out0.txt",
//              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
//              "5.0 0.0\n5.0 5.0\n0.0 10.0\n5.0 5.0\n");

//    RUN_CHECK("./generate_points.py simple 160 0.5 ~temp_out0.txt",
//              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
//              "5.0 0.0\n5.0 5.0\n0.0 10.0\n5.0 5.0\n");

    /*
     * CHECKING FOR SIZE INFLUENCE
     */

    // Two simple circles without noise
    RUN_CHECK("./generate_points.py simple 2 0.0 ~temp_out0.txt",
              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
              "0.0 0.0\n7.0 7.0\n10.0 10.0\n7.0 7.0\n");

    RUN_CHECK("./generate_points.py simple 4 0.0 ~temp_out0.txt",
              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
              "0.0 0.0\n7.0 7.0\n10.0 10.0\n7.0 7.0\n");

    RUN_CHECK("./generate_points.py simple 5 0.0 ~temp_out0.txt",
              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
              "0.0 0.0\n7.0 7.0\n10.0 10.0\n7.0 7.0\n");

    RUN_CHECK("./generate_points.py simple 8 0.0 ~temp_out0.txt",
              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
              "0.0 0.0\n7.0 7.0\n10.0 10.0\n7.0 7.0\n");

    RUN_CHECK("./generate_points.py simple 10 0.0 ~temp_out0.txt",
              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
              "0.0 0.0\n7.0 7.0\n10.0 10.0\n7.0 7.0\n");

    RUN_CHECK("./generate_points.py simple 30 0.0 ~temp_out0.txt",
              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
              "0.0 0.0\n7.0 7.0\n10.0 10.0\n7.0 7.0\n");

    // Two simple circles with 10% noise in training set
    RUN_CHECK("./generate_points.py simple 20 0.0 ~temp_out0.txt",
              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
              "0.0 0.0\n7.0 7.0\n10.0 10.0\n7.0 7.0\n");

    RUN_CHECK("./generate_points.py simple 60 0.0 ~temp_out0.txt",
              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
              "0.0 0.0\n7.0 7.0\n10.0 10.0\n7.0 7.0\n");

    // Two simple circles with 20% noise in training set
    RUN_CHECK("./generate_points.py simple 40 0.0 ~temp_out0.txt",
              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
              "0.0 0.0\n7.0 7.0\n10.0 10.0\n7.0 7.0\n");

    // Two simple circles with 30% noise in training set
    RUN_CHECK("./generate_points.py simple 60 0.0 ~temp_out0.txt",
              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
              "0.0 0.0\n7.0 7.0\n10.0 10.0\n7.0 7.0\n");

    // Two simple circles with 40% noise in training set
    RUN_CHECK("./generate_points.py simple 80 0.0 ~temp_out0.txt",
              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
              "0.0 0.0\n7.0 7.0\n10.0 10.0\n7.0 7.0\n");

    // Two simple circles with 50% noise in training set
    RUN_CHECK("./generate_points.py simple 100 0.0 ~temp_out0.txt",
              "./generate_points.py simple 160 0.0 ~temp_out1.txt",
              "0.0 0.0\n7.0 7.0\n10.0 10.0\n7.0 7.0\n");

////    // Two ellipses without noise
//    RUN_CHECK("./generate_points.py simple 150 0.0 ~temp_out0.txt ~temp_out1.txt",
//              "",
//              "0.0 0.0\n7.0 2.0\n0.0 15.0\n3.0 5.0\n");

//    // Two ellipses with 10% noise
//    RUN_CHECK("./generate_points.py simple 150 0.1 ~temp_out0.txt ~temp_out1.txt",
//              "",
//              "0.0 0.0\n7.0 2.0\n0.0 15.0\n3.0 5.0\n");

//    // Two ellipses with 20% noise
//    RUN_CHECK("./generate_points.py simple 150 0.2 ~temp_out0.txt ~temp_out1.txt",
//              "",
//              "0.0 0.0\n7.0 2.0\n0.0 15.0\n3.0 5.0\n");

//    // Two brackets (20 training set, 160 check)
//    RUN_CHECK("./generate_points.py brackets 20 0.0 ~temp_out0.txt",
//              "./generate_points.py brackets 160 0.0 ~temp_out1.txt",
//              "");

//    // Two brackets (40 training set, 160 check)
//    RUN_CHECK("./generate_points.py brackets 40 0.0 ~temp_out0.txt",
//              "./generate_points.py brackets 160 0.0 ~temp_out1.txt",
//              "");

//    // Two brackets (80 training set, 160 check)
//    RUN_CHECK("./generate_points.py brackets 80 0.0 ~temp_out0.txt",
//              "./generate_points.py brackets 160 0.0 ~temp_out1.txt",
//              "");

//    // Two brackets (160 training set, 160 check)
//    RUN_CHECK("./generate_points.py brackets 160 0.0 ~temp_out0.txt",
//              "./generate_points.py brackets 160 0.0 ~temp_out1.txt",
//              "");


//    // Two brackets (300 training set, 160 check)
//    RUN_CHECK("./generate_points.py brackets 300 0.0 ~temp_out0.txt",
//              "./generate_points.py brackets 160 0.0 ~temp_out1.txt",
//              "");

//    // Two brackets (500 training set, 160 check)
//    RUN_CHECK("./generate_points.py brackets 500 0.0 ~temp_out0.txt",
//              "./generate_points.py brackets 160 0.0 ~temp_out1.txt",
//              "");

//    // Two simple circles without noise
//    RUN_CHECK("./generate_points.py simple 50 0.0 ~temp_out0.txt ~temp_out1.txt",
//              "",
//              "0.0 0.0\n5.0 5.0\n4.0 4.0\n5.0 5.0\n");

#ifdef TIME_CHECKS
    ofstream timeStats(r.shortName() + "_stats.csv");
    timeStats << fixed; // Only fixed-point

    // Time checks
    clock_t startTime, endTime;
    double trainTime, recTime;
    int N_ITERS = 100;

    // 50 els
    TIME_CHECK("50");

    // 200 els
    N_ITERS = 50;
    TIME_CHECK("200")

    // 500 els
    N_ITERS = 10;
    TIME_CHECK("500")

    // 1000 els
    TIME_CHECK("1000")

    // 5000 els
    TIME_CHECK("5000")
#endif
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
