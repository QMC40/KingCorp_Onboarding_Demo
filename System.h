#ifndef OS_PROJECT_4_SYSTEM_H
#define OS_PROJECT_4_SYSTEM_H

#include "Matrix.h"

class System {

private:
    int row;
    int col;
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

    System(int row, int col) {
        this->row = row;
        this->col = col;
        this->processNum = 0;
        this->allocation = new Matrix(row, col);
        this->max = new Matrix(row, col);
        this->need = new Matrix(row,col);

        this->request = new Matrix(1, col);
        this->available = new Matrix(1, col);
        this->resourceRequest = new Matrix(row, col);
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

    static bool inSafeState(Matrix &need, Matrix &allocation, Matrix &available, int row);

    void populateMatrices(ifstream &infile);

    int getRow() const {
        return row;
    }

    int getCol() const {
        return col;
    }

    int getProcessNum() const {
        return processNum;
    }

};

void System::report() {
// Print the report.....................................
    printf("\nTHE REPORT STARTS HERE.........................\n\n"
           "There are %d processes in the system.\n\n"
           "There are %d resource types.\n",getRow(), getCol());
// Matrices
    allocation->print(0,"Allocation Matrix");
    max->print(0,"Max Matrix");
    need->print(0,"Need Matrix");
// Available vector
    available->print(-1,"Available Vector");
// Current status of system
    printf("\nTHE SYSTEM IS IN A %s STATE!\n",(inSafeState(*need, *allocation, *available,
        getRow())) ? "SAFE" : "NOT SAFE");
// Request Vector
    resourceRequest->setToZeroExcept(processNum, *request);
    request->print(getProcessNum(),"Request Vector");
// determine if the request is granted
    if (resourceRequest <= need) {
        if (request <= available) {
            *need -= *resourceRequest;
            *available -= *request;
            *allocation += *resourceRequest;
            if (inSafeState(*need, *allocation, *available, getRow())) {
                printf("\nTHE REQUEST CAN BE GRANTED!\n");
// calculate and print new available vector
                available->print(-1,"New Available Vector");
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

bool System::inSafeState(Matrix &need, Matrix &allocation, Matrix &available, int row) {
// Initialize variables
    Matrix work = available;
    bool finished[row]; // Finished = {False}
    for (int i = 0; i < row; i++)
        finished[i] = false;
    int last = 0;
    int current = row;
// Loop starts to determine if the current state safe
    while (last != current) {
        last = current; // update undone
        for (int i = 0; i < row; i++) {
// looking for undone process and has need </= work
            if (need.at(i) <= work && !finished[i]) {
                current--; // update update
                finished[i] = true; // set process to finished
                work += allocation.at(i); // update work
            }
        }
    }
    if (current == 0)
        return true;
    else
        return false;
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
