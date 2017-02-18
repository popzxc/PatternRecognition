#ifndef APP_HPP
#define APP_HPP

#include <map>
#include <string>
#include "Recognizers/Recognizer.hpp"
#include "Reductors/Reductor.hpp"

class App
{
public:
    App(int argc, char *argv[]);
    int exec();
private:
    void showHelp();
    void generateDataset(const std::vector<std::string> &args);
    void executeMethod(const std::vector<std::string> &args, const std::string &reductorType);

    std::vector<PointClass> readFile(std::string name);

    void runCheck(Recognizer &r, std::vector<PointClass> &trSet,
                  std::vector<PointClass> &points, std::ostream *out = nullptr);
    void defaultCheck(Recognizer &r);
    void mainCheck(Recognizer &r, Reductor &reductor, std::string trainFile,
                   std::string checkFile, std::string outputFile);
    void autoCheck(Recognizer &r);

    std::map<std::string, std::vector<std::string> > args;
    std::string trFile;
    std::string chFile;
    std::string output;
    bool runDefault;
    bool runAuto;
};

#endif // APP_HPP
