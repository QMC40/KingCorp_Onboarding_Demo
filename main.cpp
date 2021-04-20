#include <iostream>
#include <cstdio>
#include <fstream>
#include "System.h"

using namespace std;

// takes input and populated matrix
System systemBuilder(int argc, char **argv);

int main(int argc, char *argv[]) {

    // ensure name of file to be tested included in CLI
    if (argc != 2) {
        printf("command line must include ONLY the filename of the sample input data\n");
        exit(1);
    }
    systemBuilder(argc, argv);
    cout << "done";
    return 0;
}

System systemBuilder(int argc, char **argv) {

    //open up file stream
    ifstream infile;
    int row, col, processNum; // # of processes, # of resources, process # of request

// Starting to read data from file................................
    infile.open(argv[1]); // Open file
// Check error opening file
    if (infile.fail()) {
        perror(argv[1]);
        exit(1);
    }
    infile >> row; // number of processes
    infile >> col; // number of resources
    System test(row, col);

    test.populateMatrices(infile);
    test.report();





//    cout << "done";
    return test;
}