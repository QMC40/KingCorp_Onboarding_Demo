#ifndef OS_PROJECT_4_SYSTEM_H
#define OS_PROJECT_4_SYSTEM_H

#include "Matrix.h"
#include <algorithm>
#include <iostream>
/
using namespace std;

class System {

private:
    int threads;
    int resources;
    int processNum {};

    // Matrices pointers
    Matrix *max;
    Matrix *allocation;
    Matrix *need;

    // 'Vector' pointers
    Matrix::Process *available;
    Matrix *request;

public:

    System(int threads, int resources) {
        this->threads = threads;
        this->resources = resources;
        this->processNum = 0;
        this->allocation = new Matrix(threads, resources,"allocation");
        this->max = new Matrix(threads, resources,"max");
        this->need = new Matrix(threads, resources,"need");

        this->request = new Matrix(1,resources,"request");
        this->available = new Matrix::Process(resources,"available");
    }

    ~System() {
        delete allocation;
        delete max;
        delete available;
        delete request;
        delete need;
    }

    // takes input and returns system with populated matrices
    static System systemBuilder(const string& fileName);

    void systemReport() const;

    void newRequest() const;

    bool inSafeState() const;

    void populateMatrices(ifstream &infile);

    int getThreads() const {
        return threads;
    }

    int getResources() const {
        return resources;
    }

};

System System::systemBuilder(const string& fileName) {

    //open up file stream
    ifstream infile;
    int row, col; // # of processes, # of resources

    // open file for read
    infile.open(fileName); // Open file
    // fail if file doesn't open
    if (infile.fail()) {
        perror(fileName.c_str());
        exit(1);
    }
    infile >> row; // number of processes
    infile >> col; // number of resources
    auto *temp = new System(row, col);
    temp->populateMatrices(infile);
    return *temp;
}

void System::systemReport() const {

    // execute system status report
    printf("\nThere are %d processes in the system.\n\n"
           "There are %d resource types.\n", getThreads(), getResources());
    // Matrices
    allocation->print(0, "Allocation Matrix");
    max->print(0, "Max Matrix");
    need->print(0, "Need Matrix");
    // Available vector
    available->print("Available Vector");
// Current status of system
    printf("\nTHE SYSTEM IS %s STATE!\n", (inSafeState() ? "IN A SAFE" : "NOT IN A SAFE"));
}

void System::newRequest() const {
    // determine if the request is granted
    if (request->at(0) <= need->at(processNum)) {
        if (request->at(0) <= *available) {
            request->print(1,"Request Vector");
            *need->procMatrix[processNum] -= request->at(0);
            *available -= request->at(0);
            *allocation->procMatrix[processNum] += request->at(0);
            if (inSafeState()) {
                printf("\nTHE REQUEST CAN BE GRANTED!\n");
            // calculate and print new available vector
                available->print("Available Vector");
            } else
                printf("\nTHE REQUEST CANNOT BE GRANTED!\nTHE SYSTEM IS NOT IN SAFE STATE IF "
                       "REQUEST IS GRANTED!\n");
        } else
            printf("\nTHE REQUEST CANNOT BE GRANTED!\nTHE PROCESS MUST WAIT SINCE RESOURCES "
                   "ARE NOT AVAILABLE!\n");
    } else
        printf("\nTHE REQUEST CANNOT BE GRANTED!\nTHE PROCESS HAS EXCEEDED ITS MAXIMUM CLAIM!\n");
}

bool System::inSafeState() const {
    // Initialize variables
    //copy of the available resources vector
    Matrix::Process work(*available);

    // vector to mark which processes satisfied and added to safe execution order
    int finish[threads];
    for (int i = 0; i < threads; i++) {
        finish[i] = -1;
    }

    // flag to indicate if loop has completed circuit of threads without finding a process that current
    // available resources satisfies indicating unsafe state reached
    bool fail = true;
    // count of successfully allocated processes
    int count = 0;
    // counts iteration through the threads list
    int cycle = 0;

    // Loop starts to determine if the current state safe
    do {
        cycle = 0;
        fail = true;
        do {
            // TS code
//            cout << "process " << cycle << " " << (finish[cycle] == -1) << " && " << (need->at(cycle) <= *work) << endl;
            if (finish[cycle] == -1 && need->at(cycle) <= work) {
                //TS code
//                cout << "HIT " << count << " = process " << cycle << endl;
                finish[cycle] = count; // mark process as completed using count of process in go list
                work += (allocation->at(cycle)); // work regains resources released by completed process
                fail = false;   // flag set to remain in loop for another iteration
                count++;
            }
            cycle++;
        // exit if all threads successfully allocated and safe running order finished or circuit of threads
        //completed
        } while (count != threads && cycle < threads);
    // remain in the loop while processes still successfully allocated and moved to safe order
    } while (!fail);

    if (count == threads) {
        return true;
    } else {
        return false;
    }
}

void System::populateMatrices(ifstream &infile) {
    // Read input text into matrices
    allocation->read(infile);
    max->read(infile);
    available->read(infile);
    infile >> processNum;
    infile.ignore(255, ':');
    request->read(infile);
    infile.close(); // Closing file
    *need = (*max - *allocation);
}

#endif //OS_PROJECT_4_SYSTEM_H
