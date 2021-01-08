#include "eigenvalueiter.h"
#include "linearsolve.h"
//#include <cmath>
#include <cassert>
#define repo(i, l, r) for (int i = l, i##end = r; i < i##end; ++i)
using namespace LinearSolve;

// local function for implementation convenience
static inline void Assert(const Matrix &A);
static inline void Assert(const Matrix &A, const Array &b);
mreal EigenValueIter::checker_eps = 1e-6;
int EigenValueIter::max_iter = 1e5;
bool EigenValueIter::LUsuccess;
std::vector<Array> EigenValueIter::path;
static const double inf = 1.0 / 0.0;
static const int win_sz = 3;

static mreal calcEigenValue(const Array &v1, const Array &v2);

EigenValueIter::EigenPair_t
EigenValueIter::PowerMethod(Matrix A, Array init, mreal p, mreal eps) {
    if (init.empty()) {
        init = Matrix(A.row, 1);
        repo (i, 0, A.row) init(i) = 1;
    } else Assert(A, init);

    A -= p * Matrix::I(A.row);

    int idx = 1;
    Array x[win_sz];
    x[0] = init;
    x[1] = A * ((1.0 / init.norm_inf()) * init);
    double norm1, norm2;
    path.clear();
    path.push_back(x[0]);
    path.push_back(x[1]);
    repo (epoch, 0, max_iter) {
        x[(idx + 1) % win_sz] = A * ((1.0 / x[idx].norm_inf()) * x[idx]);
        idx = (idx + 1) % win_sz;
        norm1 = (x[idx] - x[(idx - 1 + win_sz) % win_sz]).norm_inf();
        norm2 = (x[idx] - x[(idx - 2 + win_sz) % win_sz]).norm_inf();
        path.push_back(x[idx]);
//        mydebug(x[idx].to_string())
//        mydebug(norm1)
//        mydebug(norm2)
        if (norm1 < eps || norm2 < eps)
            break;
    }
    x[idx] = (1.0 / x[idx].norm_inf()) * x[idx];
    return {calcEigenValue(x[idx], A * x[idx]) + p, x[idx]};
}

EigenValueIter::EigenPair_t
EigenValueIter::InversePowerMethod(Matrix A, Array init, mreal p, mreal eps) {
    if (init.empty()) {
        init = Matrix(A.row, 1);
        repo (i, 0, A.row) init(i) = 1;
    } else Assert(A, init);

    A -= p * Matrix::I(A.row);

    auto LUrst = LUDecomposition(A);
    Matrix L = LUrst.first, U = LUrst.second;
    LUsuccess = true;

    if (L.norm_inf() == inf || U.norm_inf() == inf)
        LUsuccess = false;

    int idx = 1;
    Array x[win_sz];
    x[0] = init;
    if (LUsuccess)
        x[1] = LUSolve(L, U, (1.0 / init.norm_inf()) * init);
    else
        x[1] = GaussSolve(A, (1.0 / init.norm_inf()) * init);
    double norm1, norm2;
    path.clear();
    path.push_back(x[0]);
    path.push_back(x[1]);
    repo (epoch, 0, max_iter) {
        if (LUsuccess)
            x[(idx + 1) % win_sz] = LUSolve(L, U, (1.0 / x[idx].norm_inf()) * x[idx]);
        else
            x[(idx + 1) % win_sz] = GaussSolve(A, (1.0 / x[idx].norm_inf()) * x[idx]);
        idx = (idx + 1) % win_sz;
        norm1 = (x[idx] - x[(idx - 1 + win_sz) % win_sz]).norm_inf();
        norm2 = (x[idx] - x[(idx - 2 + win_sz) % win_sz]).norm_inf();
        path.push_back(x[idx]);
//        if (epoch < 100) mydebug(x[idx].to_string())
        if (norm1 < eps || norm2 < eps)
            break;
    }
    x[idx] = (1.0 / x[idx].norm_inf()) * x[idx];
    return {calcEigenValue(x[idx], A * x[idx]) + p, x[idx]};
}


bool EigenValueIter::check(const Matrix &A, const EigenPair_t &rst, mreal eps) {
    Assert(A, rst.eigenVector);
    Array diff = A * rst.eigenVector - rst.eigenValue * rst.eigenVector;
    eps = eps <= 0 ? checker_eps : eps;
//    mydebug(diff.norm_inf())
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

static mreal calcEigenValue(const Array &v1, const Array &v2) {
    int idx = 0;
    mreal mark = abs(v1(0));
    repo (i, 1, v1.row) if (abs(v1(i)) > mark) {
        mark = v1(i);
        idx = i;
    }
    return v2(idx) / v1(idx);
}
