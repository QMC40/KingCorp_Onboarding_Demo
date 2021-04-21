#ifndef OS_PROJECT_4_SYSTEM_H
#define OS_PROJECT_4_SYSTEM_H

#include "Matrix.h"
#include <algorithm>
#include <vector>

class System {

private:
    int threads;
    int resources;
    int processNum;

    // Matrices pointers
    Matrix *max;
    Matrix *allocation;
    Matrix *need;

    // 'Vector' pointers
    Matrix *available;
    Matrix *request;
    Matrix *resourceRequest;

public:

    System(int threads, int resources) {
        this->threads = threads;
        this->resources = resources;
        this->processNum = 0;
        this->allocation = new Matrix(threads, resources,"alloc");
        this->max = new Matrix(threads, resources,"max");
        this->need = new Matrix(threads, resources,"need");

        this->request = new Matrix(1, resources,"req");
        this->available = new Matrix(1, resources,"avail");
        this->resourceRequest = new Matrix(threads, resources,"resReq");
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

    // takes input and populated matrix
    static System systemBuilder(const string& fileName);

    int getRow() const {
        return threads;
    }

    int getCol() const {
        return resources;
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

// determine if the request is granted
    if (*request <= need->at(processNum)) {
        if (request <= available) {
            request->print(1,"request");
            need->print(0, "Need Matrix before adjust");
            *need = need->at(processNum) - *request;
            need->print(0, "Need Matrix after adjust");
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
    //copy of the available resources vector
    Matrix work = available;

    // array to mark which processes satisfied and finish
    int finish[process];
    for (int i = 0; i < process; i++) {
        finish[i] = -1;
    }

    bool fail = true;
    int count = 0;
    int cycle = 0;
    vector<int> order;

    // Loop starts to determine if the current state safe
    do {
        cycle = 0;
        fail = true;
        do {
//            cout << "process " << cycle << " " << (finish[cycle] == -1) << " && " << (need.at(cycle) <= work) << endl;
            if (finish[cycle] == -1 && need.at(cycle) <= work) {
//                cout << "HIT " << count << " = process " << cycle << endl;
                order.push_back(cycle);
                finish[cycle] = count; // mark process as completed using count of process in go list
                work += allocation.at(cycle); // work regains resources released by completed process
                fail = false;
                count++;
            }
            cycle++;
        } while (count != process && cycle < process);
    } while (!fail);

    if (count == process) {
        cout << "Safe execution order is: <";
        for(int i = 0; i < order.size(); i++) {
            if(i != order.size()-1) {
                cout << order[i] << ", ";
            } else {
                cout << order[i] << ">\n";
            }
        }
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

System systemBuilder(const string& fileName) {

    //open up file stream
    ifstream infile;
    int row, col, processNum; // # of processes, # of resources, process # of request

// Starting to read data from file................................
    infile.open(fileName); // Open file
// Check error opening file
    if (infile.fail()) {
        perror(fileName.c_str());
        exit(1);
    }

    infile >> row; // number of processes
    infile >> col; // number of resources
    System temp(row, col);

    temp.populateMatrices(infile);
    temp.report();
    return temp;
}

#endif //OS_PROJECT_4_SYSTEM_H
