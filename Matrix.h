#ifndef OS_PROJECT_4_MATRIX_H
#define OS_PROJECT_4_MATRIX_H

#include <cstdio>
#include <fstream>
#include <utility>
#include <iostream>

using namespace std;

struct Matrix {

    struct Process {
        string name;
        int resources;
        int *resource;

        //constructor
        explicit Process(int resources, string name = "") {
            this->name = std::move(name);
            this->resources = resources;
            resource = new int[resources];
        }

        // copy constructor
        Process(const Process &rhs) {
            resources = rhs.resources;
            if (rhs.resources) {
                resource = new int[resources];
                for (int i = 0; i < resources; i++) {
                    resource[i] = rhs.resource[i];
                }
            }
        }

        ~Process() {
            delete[] resource;
        }

        void read(ifstream &infile) const {
            if (infile.is_open()) {
                for (int i = 0; i < resources; i++) {
                    infile >> resource[i];
                }
            }
        }

        Matrix::Process &operator=(const Matrix::Process &rhs) {
            // test for self-copy
            if (this != &rhs) {
                // copy right to left
                this->resources = rhs.resources;
                if (rhs.resource) {
                    delete[] resource;
                    resource = new int[resources];
                    for (int i = 0; i < resources; i++) {
                        this->resource[i] = rhs.resource[i];
                    }

                }
            }
            return *this;
        }

        Matrix::Process &operator+=(const Matrix::Process &rhs) {
            // Throw exception when 2 matrices have different size.
            try {
                if (rhs.resources != this->resources) {
                    throw invalid_argument("thread resources' sizes are different!\n");
                }
                // Update this threads resources
                if (this->resource && rhs.resource) {
                    for (int i = 0; i < this->resources; i++) {
                        this->resource[i] += rhs.resource[i];
                    }
                }
                return *this;
            } catch (invalid_argument &e) {
                cerr << e.what() << endl;
            }
            return *this;
        }

        Matrix::Process &operator-=(const Matrix::Process &rhs) {
            // Throw exception when 2 matrices have different size.
            try {
                if (rhs.resources != this->resources) {
                    throw invalid_argument("thread resources' sizes are different!\n");
                }
                // Update this threads resources
                if (this->resource && rhs.resource) {
                    for (int i = 0; i < this->resources; i++) {
                        this->resource[i] -= rhs.resource[i];
                    }
                }
                return *this;
            } catch (invalid_argument &e) {
                cerr << e.what() << endl;
            }
            return *this;
        }

        bool operator<=(const Matrix::Process &rhs) const {
            if (rhs.resources != this->resources) {
                cout << endl << "fail" << endl;
                return false;
            } else {
                for (int i = 0; i < resources; i++) {
                    if (this->resource[i] > rhs.resource[i]) {
                        return false;
                    }
                }
            }
            return true;
        }

        void print(const string &title) const {

            //print title and column headers for resources
            printf("\nThe %s is...\n     ", title.c_str());
            for (int i = 0; i < resources; i++) {
                printf("%*c ", 3, (char) ('A' + i));
            }
            printf("\n     ");
            // print the resource list for the process
            for (int j = 0; j < resources; j++) {
                printf("%*d ", 3, resource[j]);
            }
            printf("\n");
        }

    };

    Process **procMatrix{};

private:
    string name;
    int threads;
    int resources;


public:

// constructor
    explicit Matrix(int threads, int resources, string name) {
        this->name = std::move(name);
        this->threads = threads;
        this->resources = resources;
        procMatrix = new Process *[threads];
        for (int i = 0; i < threads; i++) {
            procMatrix[i] = new Process(resources);
        }
    }

// copy constructor
    Matrix(const Matrix &rhs) {
        threads = rhs.threads;
        resources = rhs.resources;
        if (rhs.procMatrix) {
            procMatrix = new Process *[threads];
            for (int i = 0; i < threads; i++) {
                procMatrix[i] = new Process(resources);
                if (rhs.procMatrix[i]) {
                    *procMatrix[i] = *rhs.procMatrix[i];
                }
            }
        }
    }

// destructor
    ~Matrix() {
        if (procMatrix != nullptr) {
            for (int i = 0; i < threads; i++) {
                if (procMatrix[i]) {
                    delete procMatrix[i];
                }
            }
            delete procMatrix;
        }
    }

    Process at(int r) const {
        Process ret(resources, "temp");
        try {
            if (this->procMatrix) {
                if (this->procMatrix[r]) {
                    for (int i = 0; i < resources; i++) {
                        ret.resource[i] = this->procMatrix[r]->resource[i];
                    }
                } else {
                    throw out_of_range("Matrix thread / threads is empty!\n");
                }
            } else {
                throw invalid_argument("Thread is empty!\n");
            }
        } catch (out_of_range &e) {
            e.what();
        } catch (invalid_argument &e) {
            e.what();
        }
        return ret;
    }

    // operator overloads for Matrix class

    Matrix &operator=(const Matrix &rhs) {
        // test for self-copy
        if (this != &rhs) {
            // copy right to left
            this->threads = rhs.threads;
            this->resources = rhs.resources;
            if (rhs.procMatrix) {
                delete[] procMatrix;
                procMatrix = new Process *[threads];
                for (int i = 0; i < threads; i++) {
                    procMatrix[i] = new Process(resources);
                    if (rhs.procMatrix[i]) {
                        *procMatrix[i] = *rhs.procMatrix[i];
                    }
                }
            }
        }
        return *this;
    }

    void read(ifstream &infile) const {
        if (infile.is_open()) {
            for (int i = 0; i < threads; i++) {
                for (int j = 0; j < resources; j++) {
                    infile >> procMatrix[i]->resource[j];
                }
            }
        }
    }

    Matrix &operator+=(const Matrix &rhs) {
        // Throw exception when 2 matrices have the same size.
        try {
            if (rhs.resources != this->resources && rhs.threads != this->threads) {
                throw invalid_argument("Matrices' size are different!\n");
            }
            // Update this Matrix
            if (this->procMatrix && rhs.procMatrix) {
                for (int i = 0; i < threads; i++) {
                    for (int j = 0; j < resources; j++) {
                        procMatrix[i]->resource[j] += rhs.procMatrix[i]->resource[j];
                    }
                }
                return *this;
            }
        } catch (invalid_argument &e) {
            cerr << e.what() << endl;
        }
        return *this;
    }

    Matrix operator+(const Matrix &rhs) const {
        return Matrix(*this) += rhs;
    }

    Matrix &operator-=(const Matrix &rhs) {
        // Throw exception when 2 matrices have the same size.
        try {
            if (rhs.resources != this->resources && rhs.threads != this->threads) {
                throw invalid_argument("Matrices' size are different!\n");
            }
            // Update this Matrix
            if (this->procMatrix && rhs.procMatrix) {
                for (int i = 0; i < threads; i++) {
                    for (int j = 0; j < resources; j++) {
                        procMatrix[i]->resource[j] -= rhs.procMatrix[i]->resource[j];
                    }
                }
                return *this;
            }
        } catch (invalid_argument &e) {
            cerr << e.what() << endl;
        }
        return *this;
    }

    Matrix operator-(const Matrix &rhs) const {
        return Matrix(*this) -= rhs;
    }

    bool operator<=(const Matrix &rhs) const {
        if (rhs.resources != this->resources && rhs.threads != this->threads) {
            cout << endl << "fail" << endl;
            return false;
        } else {
            for (int i = 0; i < threads; i++) {
                for (int j = 0; j < resources; j++) {
                    if (this->procMatrix[i]->resource[j] > rhs.procMatrix[i]->resource[j]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void print(int p, const string &title) const {

        //print title and column headers for resources
        printf("\nThe %s is...\n", title.c_str());
        printf("%*c  ", 3, ' ');
        for (int j = 0; j < resources; j++) {
            printf("%*c ", 3, (char) ('A' + j));
        }
        printf("\n");

        //print the actual matrix
        for (int i = 0; i < threads; i++) {

            if (p >= 0) {
                printf("%*d: ", 3, i + p);
            } else {
                printf("%*s", 5, " ");
            }
            for (int j = 0; j < resources; j++) {
                printf("%*d ", 3, procMatrix[i]->resource[j]);
            }
            printf("\n");
        }
    }

    Matrix &operator-=(const Matrix::Process &rhs) {
        // Throw exception when 2 matrices have the same size.
        try {
            if (rhs.resources != this->resources) {
                throw invalid_argument("Matrices' size are different!\n");
            }
            // Update this Matrix
            if (this->procMatrix) {
                for (int i = 0; i < threads; i++) {
                    for (int j = 0; j < resources; j++) {
                        procMatrix[i]->resource[j] -= rhs.resource[j];
                    }
                }
                return *this;
            }
        } catch (invalid_argument &e) {
            cerr << e.what() << endl;
        }
        return *this;
    }

};

#endif //OS_PROJECT_4_MATRIX_H
