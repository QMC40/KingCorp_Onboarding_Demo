#include <iostream>
#include <cstdio>
#include <unistd.h>
#include "System.h"
// This Code is Great

using namespace std;

// demonstration 2
// conflict demonstration 2

int main(int argc, char *argv[]) {

    // ensure name of file to be tested included in CLI
    if (argc != 2) {
        printf("command line must include ONLY the filename of the sample input data\n");
        exit(1);
    }
    System test = System::systemBuilder(argv[1]);
    test.systemReport();
    test.newRequest();
    return 0;
}

