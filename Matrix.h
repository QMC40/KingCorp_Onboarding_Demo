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
private:
    int row;
    int col;
    int **Mat;
    void printResource () const /* print resource type */
    {
        printf ("%*c  ", 3, ' ');
        for (int j = 0; j < col; j++)
            printf("%*c ", 3, (char) (65+j));
        printf("\n");
    }
public:
/* Constructor
Parameters:
n int number of row
m int number of column */
    explicit Matrix(int n = 1, int m = 1)
    {
        row = n;
        col = m;
        Mat = new int*[row];
        for (int i = 0; i < row; i++)
            Mat[i] = new int[col];
    }
/* Copy Constructor
Parameters:
rhs Matrix& reference to the copied matrix */
    Matrix(const Matrix& rhs)
    {
        row = rhs.row;
        col = rhs.col;
        if (rhs.Mat)
        {
            Mat = new int*[row];
            for (int i = 0; i < row; i++)
            {
                if (rhs.Mat[i])
                {
                    Mat[i] = new int[col];
                    for (int j = 0; j < row; j++)
                        Mat[i][j] = rhs.Mat[i][j];
                }
            }
        }
    }
/* Destructor */
    ~Matrix()
    {
        if (Mat)
        {
            for (int i = 0; i < row; i++)
                if (Mat[i])
                    delete [] Mat[i];
            delete [] Mat;
        }
    }
/* get row from Matrix
Return: Matrix&
Parameter:
r int the row to get */
    Matrix at(int r)
    {
        if (this->Mat)
        {
            Matrix ret(1, col);
            ret.Mat = new int*[1];
            ret.Mat[0] = new int[col];
            if (this->Mat[r])
                for (int i = 0; i < col; i++)
                    ret.Mat[0][i] = this->Mat[r][i];
            else
                throw "Matrix row is empty!\n";
            return ret;
        }
        else
            throw "Matrix is empty!\n";
    }
/* operator=
Return: Matrix&
Parameters:
rhs const Matrix& a reference to the matrix to be copied from */
    Matrix& operator=(const Matrix &rhs)
    {
// Only do assignment if RHS is a different object from this.
        if (this != &rhs)
        {
// deallocate this matrix
            if (this->Mat)
            {
                for (int i = 0; i < this->row; i++)
                    if (this->Mat[i])
                        delete[] this->Mat[i];
                delete[] this->Mat;
            }
// copy from &rhs matrix
            this->row = rhs.row;
            this->col = rhs.col;
            if (rhs.Mat)
            {
                Mat = new int *[row];
                for (int i = 0; i < row; i++)
                {
                    if (rhs.Mat[i])
                    {
                        Mat[i] = new int[col];
                        for (int j = 0; j < row; j++)
                            Mat[i][j] = rhs.Mat[i][j];
                    }
                }
            }
        }
        return *this;
    }
/* read from filestream
Parameters:
infile ifstream& reference to data file */
    void read(ifstream& infile)
    {
        if (infile.is_open())
            for (int i = 0; i < row; i++)
                for (int j = 0; j < col; j++)
                    infile >> Mat[i][j];
    }
/* set all value of Matrix to 0,
except a row r will be copied from provided 1D matrix
Parameters:
r int exceptional row
V Matrix& reference to 1D matrix to be copied */
    void setToZeroExcept(int r, Matrix& V)
    {
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                Mat[i][j] = 0;
        if (V.row == 1)
            if (r >= 0 && r < row)
                for (int j = 0; j < col; j++)
                    Mat[r][j] = V.Mat[0][j];
    }
/* operator+=
Return: Matrix&
Parameters:
rhs const Matrix& reference to the added matrix */
    Matrix& operator+= (const Matrix& rhs)
    {
// Throw exception when 2 matrices have different size.
        if (rhs.col != this->col && rhs.row != this->row)
            throw "Matrices' sizes are different!\n";
// Update this Matrix
        if (this->Mat && rhs.Mat)
            for (int i = 0; i < this->row; i++)
                for (int j = 0; j < this->col; j++)
                    this->Mat[i][j] += rhs.Mat[i][j];
        return *this;
    }
/* operator-=
Return: Matrix&
Parameters:
rhs const Matrix& reference to the added matrix */
    Matrix& operator-= (const Matrix& rhs)
    {
// Throw exception when 2 matrices have the same size.
        if (rhs.col != this->col && rhs.row != this->row)
            throw "Matrices' size are different!\n";
// Update this Matrix
        if (this->Mat && rhs.Mat)
            for (int i = 0; i < this->row; i++)
                for (int j = 0; j < this->col; j++)
                    this->Mat[i][j] -= rhs.Mat[i][j];
        return *this;
    }
/* operator+
Return: Matrix&
Parameters:
other const Matrix& reference to the added matrix */
    Matrix operator+ (const Matrix& other) const
    {
        return Matrix(*this) += other;
    }
/* operator-
Return: Matrix&
Parameters:
other const Matrix& reference to the added matrix */
    Matrix operator- (const Matrix& other) const
    {
        return Matrix(*this) -= other;
    }
/* operator<=
Return: bool
Parameters:
rhs const Matrix& reference to the comparing matrix */
    bool operator<= (const Matrix& rhs) const
    {
        if (rhs.col != this->col && rhs.row != this->row)
            return false;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                if (this->Mat[i][j] > rhs.Mat[i][j])
                    return false;
        return true;
    }
/* print matrix
Parameter:
r int the number to be start counting from
not printed if negative */
    void print (int r)
    {
        printResource();
        for (int i = 0; i < row; i++)
        {
            if (r >= 0)
                printf("%*d: ", 3, i+r);
            else
                printf("%*s", 5, " ");
            for (int j = 0; j < col; j++)
                printf("%*d ", 3, Mat[i][j]);
            printf("\n");
        }
    }
};

#endif //OS_PROJECT_4_MATRIX_H
