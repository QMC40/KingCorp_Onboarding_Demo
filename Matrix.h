#ifndef OS_PROJECT_4_MATRIX_H
#define OS_PROJECT_4_MATRIX_H

//*********************************************************
//
// Includes and Defines
//
//*********************************************************

#include <cstdio>
#include <fstream>

using namespace std;

struct Matrix {
public:
//private:
    int threads;
    int resources;
    int **subMatrix;

    // print resource / column names
    void printResource() const {
        printf("%*c  ", 3, ' ');
        for (int j = 0; j < resources; j++) {
            printf("%*c ", 3, (char) ('A' + j));
        }
        printf("\n");
    }

public:

    // constructor, defaults to one thread / row (n) and 1 resource / column (m)
    explicit Matrix(int n = 1, int m = 1) {
        threads = n;
        resources = m;
        subMatrix = new int *[threads];
        for (int i = 0; i < threads; i++) {
            subMatrix[i] = new int[resources];
        }
    }

    // copy constructor
    Matrix(const Matrix &rhs) {
        threads = rhs.threads;
        resources = rhs.resources;
        if (rhs.subMatrix) {
            subMatrix = new int *[threads];
            for (int i = 0; i < threads; i++) {
                if (rhs.subMatrix[i]) {
                    subMatrix[i] = new int[resources];
                    for (int j = 0; j < threads; j++) {
                        subMatrix[i][j] = rhs.subMatrix[i][j];
                    }
                }
            }
        }
    }

    // destructor, frees allocated memory
    ~Matrix() {
        if (subMatrix) {
            for (int i = 0; i < threads; i++) {
                if (subMatrix[i]) {
                    delete[] subMatrix[i];
                }
                delete[] subMatrix;
            }
        }
    }

    //write isEmpty() for matrix

/* get thread / row from Matrix
Return: Matrix&
Parameter:
r int the thread / row to get */
    Matrix at(int r) const {
        Matrix ret(1, resources);
        try {
            if (this->subMatrix) {
                ret.subMatrix = new int *[1];
                ret.subMatrix[0] = new int[resources];
                if (this->subMatrix[r]) {
                    for (int i = 0; i < resources; i++) {
                        ret.subMatrix[0][i] = this->subMatrix[r][i];
                    }
                } else {
                    throw out_of_range("Matrix thread / row is empty!\n");
                }
                return ret;
            } else {
                throw invalid_argument("Matrix is empty!\n");
            }
        } catch (out_of_range &e) {
            e.what();
        } catch (invalid_argument &e) {
            e.what();
        }
        return ret;
    }

    // operator overloads for Matrix class
/* operator=
Return: Matrix&
Parameters:
rhs const Matrix& a reference to the matrix to be copied from */
    Matrix &operator = (const Matrix &rhs) {
// Only do assignment if RHS is a different object from this.
        if (this != &rhs) {
// deallocate this matrix
            if (this->subMatrix) {
                for (int i = 0; i < this->threads; i++)
                    if (this->subMatrix[i])
                        delete[] this->subMatrix[i];
                delete[] this->subMatrix;
            }
// copy from &rhs matrix
            this->threads = rhs.threads;
            this->resources = rhs.resources;
            if (rhs.subMatrix) {
                subMatrix = new int *[threads];
                for (int i = 0; i < threads; i++) {
                    if (rhs.subMatrix[i]) {
                        subMatrix[i] = new int[resources];
                        for (int j = 0; j < threads; j++)
                            subMatrix[i][j] = rhs.subMatrix[i][j];
                    }
                }
            }
        }
        return *this;
    }

/* read from filestream
Parameters:
infile ifstream& reference to data file */
    void read(ifstream &infile) const {
        if (infile.is_open())
            for (int i = 0; i < threads; i++)
                for (int j = 0; j < resources; j++)
                    infile >> subMatrix[i][j];
    }

/* set all value of Matrix to 0,
except a thread / row will be copied from provided 1D matrix
Parameters:
r int exceptional thread
request Matrix& reference to 1D matrix to be copied */
    void setToZeroExcept(int r, const Matrix& request) const {
        for (int i = 0; i < threads; i++)
            for (int j = 0; j < resources; j++)
                subMatrix[i][j] = 0;
        if (request.threads == 1)
            if (r >= 0 && r < threads)
                for (int j = 0; j < resources; j++)
                    subMatrix[r][j] = request.subMatrix[0][j];
    }

/* operator+=
Return: Matrix&
Parameters:
rhs const Matrix& reference to the added matrix */
    Matrix &operator += (const Matrix &rhs) {
// Throw exception when 2 matrices have different size.
        try {
            if (rhs.resources != this->resources && rhs.threads != this->threads) {
                throw invalid_argument("Matrices' sizes are different!\n");
            }
// Update this Matrix
            if (this->subMatrix && rhs.subMatrix) {
                for (int i = 0; i < this->threads; i++) {
                    for (int j = 0; j < this->resources; j++) {
                        this->subMatrix[i][j] += rhs.subMatrix[i][j];
                    }
                }
            }
            return *this;
        } catch (invalid_argument &e) {
            cerr << e.what() << endl;
        }
        return *this;
    }

/* operator+
Return: Matrix&
Parameters:
other const Matrix& reference to the added matrix */
    Matrix operator + (const Matrix &other) const {
        return Matrix(*this) += other;
    }

/* operator-=
Return: Matrix&
Parameters:
rhs const Matrix& reference to the added matrix */
    Matrix &operator -= (const Matrix &rhs) {
//        cout << endl << "subtracting" << endl;
// Throw exception when 2 matrices have the same size.
        try {
            if (this->resources - rhs.resources + this->threads - rhs.threads != 0) {
                throw invalid_argument("Matrices' size are different!\n");
            }
// Update this Matrix
            if (this->subMatrix && rhs.subMatrix) {
                Matrix *temp;
                temp = this;
                for (int i = 0; i < temp->threads; i++) {
                    for (int j = 0; j < temp->resources; j++) {
                        temp->subMatrix[i][j] -= rhs.subMatrix[i][j];
                    }
                }
                return *temp;
            }

        } catch (invalid_argument &e) {
            cerr << e.what() << endl;
        }
        return *this;
    }

// subtraction overload
    Matrix operator - (const Matrix &other) const {
        return Matrix(*this) -= other;
    }

/* operator<=
Return: bool
Parameters:
rhs const Matrix& reference to the comparing matrix */
    bool operator <= (const Matrix &rhs) const {
        if (rhs.resources != this->resources && rhs.threads != this->threads) {
            cout << endl << "fail" << endl;
            return false;
        }
        for (int i = 0; i < threads; i++) {
            for (int j = 0; j < resources; j++) {
                if (this->subMatrix[i][j] > rhs.subMatrix[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

/* print matrix
Parameter:
r int the number to be start counting from
not printed if negative */
    void print(int r,const string& title) const {
        printf("\nThe %s is...\n",title.c_str());
        printResource();
        for (int i = 0; i < threads; i++) {
            if (r >= 0) {
                printf("%*d: ", 3, i + r);
            } else {
                printf("%*s", 5, " ");
            }
            for (int j = 0; j < resources; j++) {
                printf("%*d ", 3, subMatrix[i][j]);
            }
            printf("\n");
        }
    }

};

#endif //OS_PROJECT_4_MATRIX_H
