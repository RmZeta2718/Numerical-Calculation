#ifndef LINEARSOLVE_H
#define LINEARSOLVE_H

#include "matrix.h"
#include <utility>

//#include <iostream>
//using namespace std;
//#define debug(x) cout << __FUNCTION__ << (": " #x "\n") << (x) << endl;

namespace LinearSolve {
    // solvers
    Array GaussSolve(const Matrix &A, const Array &b);
    Array AdvancedGaussSolve(const Matrix &A, const Array &b);
    Array SquareRootSolve(const Matrix &A, const Array &b);             // A=L*L^T
    Array AdvancedSquareRootSolve(const Matrix &A, const Array &b);     // A=L*D*L^T
    Array ChaseSolve(const Array &l, const Array &c, const Array &r, const Array &b);
    Array UpperTriangularSolve(const Matrix &A, const Array &b);
    Array LowerTriangularSolve(const Matrix &A, const Array &b);

    // decompositions
    std::pair<Matrix, Matrix> LUDecomposition(const Matrix &A);
    Matrix LLTDecomposition(const Matrix &A);
    std::pair<Matrix, Array> LDLTDecomposition(const Matrix &A);

    // checkers
    bool check(const Matrix &A, const Array &x, const Array &b);
    bool check(const Array &l, const Array &c, const Array &r, const Array &x, const Array &b);
    // epsilon used for checkers
    extern mreal eps;
};


#endif // LINEARSOLVE_H
