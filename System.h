#ifndef OS_PROJECT_4_SYSTEM_H
#define OS_PROJECT_4_SYSTEM_H

#include "Matrix.h"

class System {

private:
    // nxm matrices
    Matrix *allocation;
    Matrix *max;
    Matrix *mrequest;
    Matrix *available;
    Matrix *request;

public:

    System(int row, int col) {
        this->allocation = new Matrix(row, col);
        this->max = new Matrix(row, col);
        this->mrequest = new Matrix(row, col);
        this->available = new Matrix(1, col);
        this->request = new Matrix(1, col);
    }
    ~System() {
       delete allocation;
       delete max;
       delete mrequest;
       delete available;
       delete request;
    }

////Read data into matrices and vectors
//    allocation.read(infile);
//    max.read(infile);
//    available.read(infile);
//    infile >>processNum;
//    infile.ignore(255, ':');
//    request.read(infile);
//    infile.close(); // Closing file
//    Matrix need = (max - allocation);
//    mrequest.setToZeroExcept(processNum, request);
};


#endif //OS_PROJECT_4_SYSTEM_H
