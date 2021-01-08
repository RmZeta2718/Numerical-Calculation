#include "linearsolveiter.h"
#include <cmath>
#include <cassert>
#define repo(i, l, r) for (int i = l, i##end = r; i < i##end; ++i)


// local function for implementation convenience
static inline void Assert(const Matrix &A);
static inline void Assert(const Matrix &A, const Array &b);
mreal LinearSolveIter::checker_eps = 1e-6;
int LinearSolveIter::max_iter = 1e5;
std::vector<Array> LinearSolveIter::path;
static const double inf = 1.0 / 0.0;

Array LinearSolveIter::JacobiSolve(const Matrix &A, const Array &b, mreal eps, const Array &init)
{
    Assert(A, b);
    if (!init.empty())
        Assert(A, init);
    int n = A.row;
    std::vector<mreal> vi(n), vdinv(n);
    repo (i, 0, n) {
        vi[i] = 1;
        vdinv[i] = 1 / A(i, i);
    }
    Matrix I = Matrix::Diag(vi), Dinv = Matrix::Diag(vdinv);
    Matrix B = I - Dinv * A;
    Array g = Dinv * b;
    Array x1, x2 = init.empty() ? Matrix(n, 1) : init;
    double norm;
    path.clear();
    repo (epoch, 0, max_iter) {
        path.push_back(x2);
        x1 = x2;
        x2 = B * x1 + g;
        norm = (x1 - x2).norm_inf();
        if (norm < eps)
            break;
        if (norm == inf)
            return Matrix();
    }
    return x2;
}

Array LinearSolveIter::GaussSeidelSolve(const Matrix &A, const Array &b, mreal eps, const Array &init)
{
    Assert(A, b);
    if (!init.empty())
        Assert(A, init);
    int n = A.row;
    Array x1, x2 = init.empty() ? Matrix(n, 1) : init;
    double norm;
    path.clear();
    repo (epoch, 0, max_iter) {
        path.push_back(x2);
        x1 = x2;
        repo (i, 0, n) {
            mreal sum = b(i);
            repo (j, 0, n) if (j != i)
                sum -= A(i, j) * x2(j);
            x2(i) = sum / A(i, i);
        }
        norm = (x1 - x2).norm_inf();
        if (norm < eps)
            break;
        if (norm == inf)
            return Matrix();
    }
    return x2;
}

Array LinearSolveIter::SORSolve(const Matrix &A, const Array &b, mreal omega, mreal eps, const Array &init)
{
    Assert(A, b);
    if (!init.empty())
        Assert(A, init);
    int n = A.row;
    Array x1, x2 = init.empty() ? Matrix(n, 1) : init;
    double norm;
    path.clear();
    repo (epoch, 0, max_iter) {
        path.push_back(x2);
        x1 = x2;
        repo (i, 0, n) {
            mreal sum = b(i);
            repo (j, 0, n) if (j != i)
                sum -= A(i, j) * x2(j);
            x2(i) = omega * sum / A(i, i) + (1 - omega) * x1(i);
        }
        norm = (x1 - x2).norm_inf();
        if (norm < eps)
            break;
        if (norm == inf)
            return Matrix();
    }
    return x2;
}

bool LinearSolveIter::check(const Matrix &A, const Array &x, const Array &b, mreal eps)
{
    Assert(A, b);
    Assert(A, x);
    Array diff = A * x - b;
    eps = eps <= 0 ? checker_eps : eps;
    return diff.norm_inf() < eps;
}

static inline void Assert(const Matrix &A) {
    // need square matrix
    assert(A.row == A.col);
}

static inline void Assert(const Matrix &A, const Array &b) {
    Assert(A);
    assert(A.row == b.row);
    assert(b.col == 1);
}

