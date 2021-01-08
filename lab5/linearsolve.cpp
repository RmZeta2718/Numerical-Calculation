#include "linearsolve.h"
#include <cmath>
#include <cassert>
#define repo(i, l, r) for (int i = l, i##end = r; i < i##end; ++i)

// local function for implementation convenience
static inline void Assert(const Matrix &A);
static inline void Assert(const Matrix &A, const Array &b);
static inline void Assert(const Array &l, const Array &c, const Array &r, const Array &b);
static void SwapRow(Matrix &A, Array &b, int row1, int row2);
static bool GaussElim(Matrix &A, Array &b, int type);
#define GAUSS_FIND_NOT_ZERO 0
#define GAUSS_FIND_MAX 1
mreal LinearSolve::eps = 1e-8;

Array LinearSolve::GaussSolve(const Matrix &A, const Array &b)
{
    Assert(A, b);
    Matrix tA(A);
    Array tb(b);
    if (!GaussElim(tA, tb, GAUSS_FIND_NOT_ZERO))
        return Array();

    return UpperTriangularSolve(tA, tb);
}

Array LinearSolve::AdvancedGaussSolve(const Matrix &A, const Array &b)
{
    Assert(A, b);
    Matrix tA(A);
    Array tb(b);
    if (!GaussElim(tA, tb, GAUSS_FIND_MAX))
        return Array();

    return UpperTriangularSolve(tA, tb);
}

Array LinearSolve::SquareRootSolve(const Matrix &A, const Array &b)
{
    Assert(A, b);
    Matrix L = LLTDecomposition(A);
    Array y = LowerTriangularSolve(L, b);
    return UpperTriangularSolve(L.T(), y);
}

Array LinearSolve::AdvancedSquareRootSolve(const Matrix &A, const Array &b)
{
    Assert(A, b);
    auto p = LDLTDecomposition(A);
    Matrix L = p.first;
    Array D = p.second;
    Array y = LowerTriangularSolve(L, b);
    repo (i, 0, y.row) y(i) /= D(i);
    return UpperTriangularSolve(L.T(), y);
}

Array LinearSolve::ChaseSolve(const Array &l, const Array &c, const Array &r, const Array &b)
{
    Assert(l, c, r, b);
    int n = c.row;
    Array u(n, 1), v(n - 1, 1), x(n, 1);
    u(0) = c(0);
    v(0) = r(0) / u(0);
    x(0) = b(0) / u(0);
    repo (i, 1, n - 1) {
        u(i) = c(i) - l(i - 1) * v(i - 1);
        v(i) = r(i) / u(i);
        x(i) = (b(i) - l(i - 1) * x(i - 1)) / u(i);
    }
    u(n - 1) = c(n - 1) - l(n - 2) * v(n - 2);
    x(n - 1) = (b(n - 1) - l(n - 2) * x(n - 2)) / u(n - 1);
    for (int i = n - 2; i >= 0; --i)
        x(i) -= v(i) * x(i + 1);
    return x;
}

Array LinearSolve::UpperTriangularSolve(const Matrix &A, const Array &b)
{
    Assert(A);
    Array rst(b);
    int n = A.row;
    for (int i = n - 1; i >= 0; i--) {
        repo (j, i + 1, n)
            rst(i) -= A(i, j) * rst(j);
        rst(i) /= A(i, i);
    }
    return rst;
}

Array LinearSolve::LowerTriangularSolve(const Matrix &A, const Array &b)
{
    Array rst(b);
    int n = A.row;
    repo (i, 0, n) {
        repo (j, 0, i)
            rst(i) -= A(i, j) * rst(j);
        rst(i) /= A(i, i);
    }
    return rst;
}

Array LinearSolve::LUSolve(const Matrix& L, const Matrix& U, const Array& b) {
    Array x = LowerTriangularSolve(L, b);
    return UpperTriangularSolve(U, x);
}

std::pair<Matrix, Matrix> LinearSolve::LUDecomposition(const Matrix &A)
{
    // Doolittle method
    Assert(A);
    int n = A.row;
    Matrix L(n, n), U(n, n);
    // Decomposing matrix into Upper and Lower triangular matrix
    repo (k, 0, n) {
        // Upper Triangular
        repo (j, k, n) {
            // Summation of L(i, k) * U(k, j)
            mreal sum = 0;
            repo (r, 0, k)
                sum += L(k, r) * U(r, j);
            // Evaluating U(i, k)
            U(k, j) = A(k, j) - sum;
        }
        // Lower Triangular
        L(k, k) = 1;
        repo (i, k + 1, n) {
            // Summation of L(j, k) * U(k, i)
            mreal sum = 0;
            repo (r, 0, k)
                sum += (L(i, r) * U(r, k));
            // Evaluating L(j, i)
            L(i, k) = (A(i, k) - sum) / U(k, k);
        }
    }
    return {L, U};
}

Matrix LinearSolve::LLTDecomposition(const Matrix &A)
{
    Assert(A);
    int n = A.row;
    Matrix L(n, n);
    repo (i, 0, n) {
        repo (j, 0, i) {
            mreal sum = 0;
            repo (k, 0, j)
                sum += L(i, k) * L(j, k);
            L(i, j) = (A(i, j) - sum) / L(j, j);
        }
        mreal sum = 0;
        repo (k, 0, i)
            sum += L(i, k) * L(i, k);
        L(i, i) = sqrt(A(i, i) - sum);
    }
    return L;
}

std::pair<Matrix, Array> LinearSolve::LDLTDecomposition(const Matrix &A)
{
    Assert(A);
    int n = A.row;
    Matrix L(n, n);
    Array D(n, 1);
    repo (i, 0, n) {
        L(i, i) = 1;
        repo (j, 0, i) {
            mreal sum = 0;
            repo (k, 0, j)
                sum += D(k) * L(i, k) * L(j, k);
            L(i, j) = (A(i, j) - sum) / D(j);
        }
        mreal sum = 0;
        repo (k, 0, i)
            sum += D(k) * L(i, k) * L(i, k);
        D(i) = A(i, i) - sum;
    }
    return {L, D};
}


bool LinearSolve::check(const Matrix &A, const Array &x, const Array &b)
{
    Assert(A, b);
    Assert(A, x);
    Array diff = A * x - b;
    repo (i, 0, diff.row * diff.col)
        if (std::abs(diff.num[i]) > eps)
            return false;
    return true;
}

bool LinearSolve::check(const Array &l, const Array &c, const Array &r, const Array &x, const Array &b)
{
    Assert(l, c, r, b);
    assert(x.row == b.row && x.col == 1);
    int n = x.row;
    if (c(0) * x(0) + r(0) * x(1) - b(0) > eps)
        return false;
    repo (i, 1, n - 1)
        if (l(i - 1) * x(i - 1) + c(i) * x(i) + r(i) * x(i + 1) - b(i) > eps)
            return false;
    if (l(n - 2) * x(n - 2) + c(n - 1) * x(n - 1) - b(n - 1) > eps)
        return false;
    return true;
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
static inline void Assert(const Array &l, const Array &c, const Array &r, const Array &b) {
    assert(l.row == r.row);
    assert(b.row == c.row);
    assert(c.row == l.row + 1);
    assert(l.col == 1 && c.col == 1 && r.col == 1 && b.col == 1);
}

static void SwapRow(Matrix &A, Array &b, int row1, int row2)
{
    repo (j, 0, A.col)
        std::swap(A(row1, j), A(row2, j));
    std::swap(b(row1), b(row2));
}

static bool GaussElim(Matrix &A, Array &b, int type)
{
    int n = A.row;
    repo (k, 0, n - 1) {
        // Initialize index for pivot
        int idx = k;
        switch (type) {
            case GAUSS_FIND_NOT_ZERO:
                // find none zero amplitude for pivot if any
                while (++idx < n && std::abs(A(idx, k)) < LinearSolve::eps);
                break;
            case GAUSS_FIND_MAX:
                // find greater amplitude for pivot if any
                repo (i, k + 1, n)
                    if (std::abs(A(i, k)) > std::abs(A(idx, k)))
                        idx = i;
                break;
        }
        /* if a prinicipal diagonal element  is zero,
         * it denotes that matrix is singular, and
         * will lead to a division-by-zero later. */
        if (idx >= n || std::abs(A(idx, k)) < LinearSolve::eps)
            return false;
        // Swap the greatest value row with current row
        if (idx != k)
            SwapRow(A, b, k, idx);

        repo (i, k + 1, n) {
            /* factor f to set current row kth element to 0,
             * and subsequently remaining kth column to 0 */
            mreal f = A(i, k) / A(k, k);
            /* subtract fth multiple of corresponding kth
               row element*/
            repo (j, k + 1, n)
                A(i, j) -= A(k, j) * f;
            b(i) -= b(k) * f;
            /* filling lower triangular matrix with zeros*/
            A(i, k) = 0;
        }
    }
    return true;
}


