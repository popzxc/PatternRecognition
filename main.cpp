#include <iostream>
#include <fstream>
#include "LabManager.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    LabManager manager;
    manager.executeLab(argc, argv);

    return 0;
}
