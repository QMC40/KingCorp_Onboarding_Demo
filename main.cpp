#include <iostream>
#include <cstdio>
#include <fstream>
#include "System.h"

using namespace std;

int main(int argc, char *argv[]) {

    string fileName = *argv;
    // ensure name of file to be tested included in CLI
    if (argc != 2) {
        printf("command line must include ONLY the filename of the sample input data\n");
        exit(1);
    }

    System test = System::systemBuilder(fileName);
    cout << "done";
    return 0;
}

