#include "matrix.h"
#include <cassert>
#include <cmath>
#include <iostream>
#define repo(i, l, r) for (int i = l, i##end = r; i < i##end; ++i)

Matrix::Matrix():row(0), col(0)
{
    num.resize(0);
}

Matrix::Matrix(const int row, const int col):row(row), col(col)
{
    assert(row > 0 && col > 0);
    num.resize(row * col);
    clear();
}

Matrix::Matrix(const Matrix &mat):row(mat.row), col(mat.col)
{
    num = mat.num;
}

mreal Matrix::operator()(int i, int j) const
{
    assert(i * col + j < row * col);
    return num[i * col + j];
}

mreal &Matrix::operator()(int i, int j)
{
    assert(i * col + j < row * col);
    return num[i * col + j];
}

Matrix &Matrix::operator+=(const Matrix &rhs)
{
    return *this = *this + rhs;
}

Matrix &Matrix::operator-=(const Matrix &rhs)
{
    return *this = *this - rhs;
}

Matrix &Matrix::operator*=(const Matrix &rhs)
{
    return *this = *this * rhs;
}

Matrix Matrix::T() const
{
    Matrix rst(col, row);
    repo (i, 0, row) repo (j, 0, col)
        rst(j, i) = (*this)(i, j);
    return rst;
}

int Matrix::size() const
{
    return row * col;
}

bool Matrix::empty() const
{
    return row * col == 0;
}

std::string Matrix::to_string() const
{
    std::string rst;
    if (row * col == 0) rst = "Empty Matrix\n";
    else repo (i, 0, row * col)
        rst += std::to_string(num[i]) + (i % col != col - 1 ? ' ' : '\n');
    return rst;
}

mreal Matrix::norm_2() const
{
    mreal rst = 0;
    repo (i, 0, row * col)
        rst += num[i] * num[i];
    return std::sqrt(rst);
}

mreal Matrix::norm_inf() const
{
    assert(row * col);
    mreal rst = std::abs(num[0]);
    repo (i, 1, row * col)
            rst = std::max(rst, std::abs(num[i]));
    return rst;
}

void Matrix::clear()
{
    repo (i, 0, row * col) num[i] = 0;
}

Matrix Matrix::I(int n)
{
    Matrix rst(n, n);
    repo (i, 0, n)
        rst(i, i) = 1;
    return rst;
}

Matrix Matrix::Diag(const std::vector<mreal> &v)
{
    Matrix rst(v.size(), v.size());
    repo (i, 0, v.size())
        rst(i, i) = v[i];
    return rst;
}

Matrix Matrix::Array(const std::vector<mreal> &v)
{
    Matrix rst(v.size(), 1);
    repo (i, 0, v.size())
        rst.num[i] = v[i];
    return rst;
}


Matrix operator+(const Matrix& matrix1, const Matrix& matrix2) {
    assert(matrix1.row == matrix2.row);
    assert(matrix1.col == matrix2.col);
    Matrix rst(matrix1.row, matrix1.col);

    repo (i, 0, rst.row * rst.col)
        rst.num[i] = matrix1.num[i] + matrix2.num[i];
    return rst;
}

Matrix operator-(const Matrix& matrix1, const Matrix& matrix2) {
    assert(matrix1.row == matrix2.row);
    assert(matrix1.col == matrix2.col);
    Matrix rst(matrix1.row, matrix1.col);

    repo (i, 0, rst.row * rst.col)
        rst.num[i] = matrix1.num[i] - matrix2.num[i];
    return rst;
}

Matrix operator*(const Matrix& matrix1, const Matrix& matrix2) {
    assert(matrix1.col == matrix2.row);
    int mid = matrix1.col;
    Matrix rst(matrix1.row, matrix2.col);

    repo (i, 0, rst.row) repo (j, 0, rst.col) repo (k, 0, mid)
        rst(i, j) += matrix1(i, k) * matrix2(k, j);
    return rst;
}

Matrix operator*(mreal c, const Matrix& matrix) {
    Matrix rst(matrix.row, matrix.col);
    repo (i, 0, rst.row * rst.col)
        rst.num[i] = matrix.num[i] * c;
    return rst;
}
