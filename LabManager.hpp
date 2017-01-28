#ifndef LABMANAGER_HPP
#define LABMANAGER_HPP

#include <map>
#include <fstream>
#include <string>
#include "Recognizer.hpp"

class LabManager
{
public:
    LabManager();
    void executeLab(int argc, char *argv[]);
private:
    void executeLab1();
    void executeLab2();
    void executeLab3();
    void executeLab4();

    std::vector<PointClass> readFile(std::string name);

    void runCheck(Recognizer &r, std::vector<PointClass> &trSet,
                  std::vector<PointClass> &points, std::ostream *out = nullptr);
    void defaultCheck(Recognizer &r);
    void mainCheck(Recognizer &r, std::string trainFile, std::string checkFile, std::string outputFile);

    std::map<std::string, std::vector<std::string> > args;
};

#endif // LABMANAGER_HPP
