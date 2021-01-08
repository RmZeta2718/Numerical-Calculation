#ifndef Matrix_H
#define Matrix_H

#include <vector>
#include <string>
typedef double mreal;

class Matrix
{
public:
    // data
    int row, col;
    std::vector<mreal> num;

    // constructors
    Matrix();
    Matrix(const int row, const int col);
    Matrix(const Matrix& mat);

    // operators
    mreal operator()(int i, int j = 0) const;
    mreal &operator()(int i, int j = 0);
    Matrix &operator+=(const Matrix& rhs);
    Matrix &operator-=(const Matrix& rhs);
    Matrix &operator*=(const Matrix& rhs);

    // const methods
    Matrix T() const;
    int size() const;
    bool empty() const;             // whether matrix size is 0x0
    std::string to_string() const;  // transfer matrix to std::string
    mreal norm_2() const;           // return the infinity norm of the matrix
    mreal norm_inf() const;         // return the infinity norm of the matrix

    // manipulators
    void clear();

    // static functions
    static Matrix I(int n);
    // create diagnal matrix using vector
    static Matrix Diag(const std::vector<mreal> &v);
    // create nx1 Array using vector
    static Matrix Array(const std::vector<mreal> &v);

    // friend functions
    friend Matrix operator+(const Matrix& Matrix1,const Matrix& Matrix2);
    friend Matrix operator-(const Matrix& Matrix1,const Matrix& Matrix2);
    friend Matrix operator*(const Matrix& matrix1, const Matrix& matrix2);
    friend Matrix operator*(mreal c, const Matrix& matrix);
};

// use Matrix as Array for simplicity
typedef Matrix Array;


#endif // Matrix_H
