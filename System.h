#ifndef OS_PROJECT_4_SYSTEM_H
#define OS_PROJECT_4_SYSTEM_H

#include "Matrix.h"
#include <algorithm>

class System {

private:
    int threads;
    int resources;
    int processNum;

    // Matrices pointers
    Matrix *max;
    Matrix *allocation;
    Matrix *need;

    // Vector pointers
    Matrix *available;
    Matrix *request;
    Matrix *resourceRequest;

public:

    System(int threads, int resources) {
        this->threads = threads;
        this->resources = resources;
        this->processNum = 0;
        this->allocation = new Matrix(threads, resources);
        this->max = new Matrix(threads, resources);
        this->need = new Matrix(threads, resources);

        this->request = new Matrix(1, resources);
        this->available = new Matrix(1, resources);
        this->resourceRequest = new Matrix(threads, resources);
    }

    ~System() {
        delete allocation;
        delete max;
        delete resourceRequest;
        delete available;
        delete request;
        delete need;
    }

    void report();

    void newRequest();

    static bool inSafeState(Matrix &need, Matrix &allocation, Matrix &available, int process);

    void populateMatrices(ifstream &infile);

    int getRow() const {
        return threads;
    }

    int getCol() const {
        return resources;
    }

    int getProcessNum() const {
        return processNum;
    }

};

void System::report() {
// Print the report.....................................
    printf("\nTHE REPORT STARTS HERE.........................\n\n"
           "There are %d processes in the system.\n\n"
           "There are %d resource types.\n", getRow(), getCol());
// Matrices
    allocation->print(0, "Allocation Matrix");
    max->print(0, "Max Matrix");
    need->print(0, "Need Matrix");
// Available vector
    available->print(-1, "Available Vector");
// Current status of system
    printf("\nTHE SYSTEM IS %s STATE!\n", (inSafeState(*need, *allocation, *available,
                                                            getRow())) ? "IN A SAFE" : "NOT IN A SAFE");
    newRequest();

}

void System::newRequest() {
    // Request Vector
    Matrix reqNeed = need->at(processNum);

    reqNeed.print(1,"req test");
    resourceRequest->setToZeroExcept(processNum, *request);
//    resourceRequest->setToZeroExcept(processNum, *request);
    request->print(getProcessNum(), "Request Vector");
// determine if the request is granted
    if (request <= &reqNeed) {
        if (request <= available) {
            request->print(1,"request");
            available->print(1,"available");
            *need -= *resourceRequest;
            *available -= *request;
            *allocation += *resourceRequest;
            if (inSafeState(*need, *allocation, *available, getRow())) {
                printf("\nTHE REQUEST CAN BE GRANTED!\n");
// calculate and print new available vector
                available->print(-1, "New Available Vector");
            } else
                printf("\nTHE REQUEST CANNOT BE GRANTED!\nTHE SYSTEM IS NOT IN SAFE STATE IF "
                       "REQUEST IS GRANTED!\n");
        } else
            printf("\nTHE REQUEST CANNOT BE GRANTED!\nTHE PROCESS MUST WAIT SINCE RESOURCES "
                   "ARE NOT AVAILABLE!\n");
    } else
        printf("\nTHE REQUEST CANNOT BE GRANTED!\nTHE PROCESS HAS EXCEEDED ITS MAXIMUM CLAIM!\n");
    printf("\nEND REPORT......................................\n\n");
}


bool System::inSafeState(Matrix &need, Matrix &allocation, Matrix &available, int process) {
    // Initialize variables
    //copy of the available resources vector
    Matrix work = available;

    // vector to mark which processes satisfied and finish
    int finish[process];
    for (int i = 0; i < process; i++) {
        finish[i] = -1;
    }


    bool fail = true;
    int count = 0;
    int cycle = 0;

    // Loop starts to determine if the current state safe
    do {
        cycle = 0;
        fail = true;
        do {
//            cout << "cycle " << cycle << " " << (finish[cycle] == -1) << " && " << (need.at(cycle) <= work) << endl;
            if (finish[cycle] == -1 && need.at(cycle) <= work) {
//                cout << "HIT " << count << " = cycle " << cycle << endl;
                finish[cycle] = count; // mark process as completed using count of process in go list
                work += allocation.at(cycle); // work regains resources released by completed process
                fail = false;
                count++;
            }
            cycle++;
        } while (cycle < process);
    } while (!fail);

    if (count == process) {
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
