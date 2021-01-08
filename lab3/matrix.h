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
    bool empty() const;
    std::string to_string() const;

    // manipulators
    void clear();

    // friend functions
    friend Matrix operator+(const Matrix& Matrix1,const Matrix& Matrix2);
    friend Matrix operator-(const Matrix& Matrix1,const Matrix& Matrix2);
    friend Matrix operator*(const Matrix& matrix1, const Matrix& matrix2);
};

// use Matrix as Array for simplicity
typedef Matrix Array;


#endif // Matrix_H
