#include <iostream>
#include <cstdio>
#include <fstream>
#include "System.h"

using namespace std;

// takes input and populated matrix
System systemBuilder(char **argv);

int main(int argc, char *argv[]) {

    // ensure name of file to be tested included in CLI
    if (argc != 2) {
        printf("command line must include ONLY the filename of the sample input data\n");
        exit(1);
    }

    System test = systemBuilder(argv);
    test.report();
    test.newRequest();
    return 0;
}

System systemBuilder(char **argv) {

    //open up file stream
    ifstream infile;
    int row, col;

// Starting to read data from file................................
    infile.open(argv[1]); // Open file
// Check error opening file
    if (infile.fail()) {
        perror(argv[1]);
        exit(1);
    }
    infile >> row; // number of processes
    infile >> col; // number of resources
    System temp(row, col);
    temp.populateMatrices(infile);
    return temp;
}