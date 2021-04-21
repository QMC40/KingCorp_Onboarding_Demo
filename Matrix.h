#ifndef OS_PROJECT_4_MATRIX_H
#define OS_PROJECT_4_MATRIX_H

#include <cstdio>
#include <fstream>
#include <utility>

using namespace std;

struct Matrix {

    struct Process {
        string name;
        int resources;
        int *resource;

        Process(int resources, string name) : resources{resources}, name{std::move(name)} {
            resource = new int[resources];
        }

        explicit Process(int resources) : Process{resources, ""} {}

        // copy constructor
        Process(const Process &rhs) {
            name = rhs.name;
            resources = rhs.resources;

            for (int i = 0; i < resources; i++) {
                if (rhs.procMatrix[i]) {
                    procMatrix[i] = new Process(resources);
                    for (int j = 0; j < resources; j++) {
                        procMatrix[i]->resource[j] = rhs.procMatrix[i]->resource[j];
                    }
                }
            }
        }

        ~Process() {
//            printf("Process destruct\n");
            delete[] resource;
        }

        bool operator<=(const Process &rhs) const {
            // Throw exception when 2 Processes have different sizes.
            try {
                if (rhs.resources != this->resources) {
                    throw invalid_argument("Processes' sizes are different!\n");
                } else {
                    for (int i = 0; i < resources; i++) {
                        if (this->resource[i] > rhs.resource[i]) {
                            return false;
                        }
                    }
                    return true;
                }
            } catch (invalid_argument &e) {
                cerr << e.what() << endl;
                return false;
            }
        }

        Process &operator+=(const Process &rhs) {
            // Throw exception when 2 matrices have different size.
            try {
                if (this->resources != rhs.resources) {
                    throw invalid_argument("Processes' sizes are different!\n");
                }
                // Update this Process
                for (int i = 0; i < this->resources; i++) {
                    {
                        this->resource[i] += rhs.resource[i];
                    }
                }
                return *this;
            } catch (invalid_argument &e) {
                cerr << e.what() << endl;
            }
            return *this;
        }

        Process operator+(const Process &other) const {
            return Process(*this) += other;
        }

        Process &operator-=(const Process &rhs) {

            try {
                if (this->resources - rhs.resources != 0) {
                    throw invalid_argument("Processes' sizes are different!\n");
                }
                // Update this Process
                for (int i = 0; i < this->resources; i++) {
                    this->resource[i] -= rhs.resource[i];
                }
                return *this;

            } catch (invalid_argument &e) {
                cerr << e.what() << endl;
            }
            return *this;
        }

        Process operator-(const Process &other) const {
            return Process(*this) -= other;
        }

        void print(int p) const {

            //print title and column headers for resources
            if (!name.empty()) {
                printf("\nThe %s is...\n", getName().c_str());
                printf("%*c  ", 3, ' ');
                for (int j = 0; j < resources; j++) {
                    printf("%*c ", 3, (char) ('A' + j));
                }
            }

            printf("\n%*s", 5, " ");

            for (int i = 0; i < resources; i++) {
                printf("%*d ", 3, resource[i]);
            }
            printf("\n");
        }

        const string &getName() const {
            return name;
        }

        int getResources() const {
            return resources;
        }

    };

private:
    string name;
    int threads;
    int resources;
    Process **procMatrix;

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

//    // constructor
//    explicit Matrix(int threads, int resources) {
//        this->threads = threads;
//        this->resources = resources;
//        procMatrix = new int* [threads];
//        for (int i = 0; i < threads; i++) {
//            procMatrix[i] = new int[resources];
//        }
//    }

    // copy constructor
    Matrix(const Matrix &rhs) {
        threads = rhs.threads;
        resources = rhs.resources;
        if (rhs.procMatrix) {
            procMatrix = new Process *[threads];
            for (int i = 0; i < threads; i++) {
                if (rhs.procMatrix[i]) {
                    procMatrix[i] = new Process(resources);
                    for (int j = 0; j < resources; j++) {
                        procMatrix[i]->resource[j] = rhs.procMatrix[i]->resource[j];
                    }
                }
            }
        }
    }

    // destructor
    ~Matrix() {
        if (procMatrix != nullptr) {
//            printf("%s destruct\n",name.c_str());
            for (int i = 0; i < threads; i++) {
                if (procMatrix[i]) {
                    delete procMatrix[i];
                }
            }
            delete procMatrix;
        }
    }

    //write isEmpty() for matrix

    Matrix at(int r) const {
        Matrix ret(1, resources, "temp");
        try {
            if (this->procMatrix) {
                ret.procMatrix = new Process *[1];
                ret.procMatrix[0] = new Process(resources);
                if (this->procMatrix[r]) {
                    for (int i = 0; i < resources; i++) {
                        ret.procMatrix[0]->resource[i] = this->procMatrix[r]->resource[i];
                    }
                } else {
                    throw out_of_range("Matrix thread / threads is empty!\n");
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

    Matrix &operator=(const Matrix &rhs) {
        // test for self-copy
        if (this != &rhs) {
            // delete old matrix
            delete procMatrix;

            // copy right to left
            this->threads = rhs.threads;
            this->resources = rhs.resources;
            if (rhs.procMatrix) {
                procMatrix = new Process *[threads];
                for (int i = 0; i < threads; i++) {
                    if (rhs.procMatrix[i]) {
                        procMatrix[i] = new Process(resources);
                        for (int j = 0; j < threads; j++)
                            procMatrix[i]->resource[j] = rhs.procMatrix[i]->resource[j];
                    }
                }
            }
        }
        return *this;
    }

    void read(ifstream &infile) const {
        int temp;
        if (infile.is_open()) {
            for (int i = 0; i < threads; i++) {
                for (int j = 0; j < resources; j++) {
                    infile >> procMatrix[i]->resource[j];
                }
            }
        }
    }

    void setToZeroExcept(int r, const Matrix &request) const {
        for (int i = 0; i < threads; i++) {
            for (int j = 0; j < resources; j++) {

                if (i == r) {
                    procMatrix[r]->resource[j] = request.procMatrix[0]->resource[j];
                } else {
                    procMatrix[i]->resource[j] = 0;
                }
            }
        }
    }

    Matrix &operator+=(const Matrix &rhs) {
// Throw exception when 2 matrices have different size.
        try {
            if (rhs.resources != this->resources && rhs.threads != this->threads) {
                throw invalid_argument("Matrices' sizes are different!\n");
            }
// Update this Matrix
            if (this->procMatrix && rhs.procMatrix) {
                for (int i = 0; i < this->threads; i++) {
                    for (int j = 0; j < this->resources; j++) {
                        this->procMatrix[i]->resource[j] += rhs.procMatrix[i]->resource[j];
                    }
                }
            }
            return *this;
        } catch (invalid_argument &e) {
            cerr << e.what() << endl;
        }
        return *this;
    }

    Matrix operator+(const Matrix &other) const {
        return Matrix(*this) += other;
    }

    Matrix &operator-=(const Matrix &rhs) {
//        cout << endl << "subtracting" << endl;
// Throw exception when 2 matrices have the same size.
        try {
            if (this->resources - rhs.resources + this->threads - rhs.threads != 0) {
                throw invalid_argument("Matrices' size are different!\n");
            }
// Update this Matrix
            if (this->procMatrix && rhs.procMatrix) {
                Matrix *temp;
                temp = this;
                for (int i = 0; i < temp->threads; i++) {
                    for (int j = 0; j < temp->resources; j++) {
                        temp->procMatrix[i]->resource[j] -= rhs.procMatrix[i]->resource[j];
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
    Matrix operator-(const Matrix &other) const {
        return Matrix(*this) -= other;
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

};

#endif //OS_PROJECT_4_MATRIX_H
