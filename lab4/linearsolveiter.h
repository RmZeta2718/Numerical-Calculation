#ifndef LINEARSOLVEITER_H
#define LINEARSOLVEITER_H

#include "matrix.h"

//#include <iostream>
//using namespace std;
//#define mydebug(x) cout << __FUNCTION__ << (": " #x "\n") << (x) << endl;

namespace LinearSolveIter
{
    // solvers
    Array JacobiSolve(const Matrix &A, const Array &b,
                      mreal eps = 1e-6, const Array &init = Matrix());
    Array GaussSeidelSolve(const Matrix &A, const Array &b,
                           mreal eps = 1e-6, const Array &init = Matrix());
    Array SORSolve(const Matrix &A, const Array &b, mreal omega,
                   mreal eps = 1e-6, const Array &init = Matrix());

    // checkers
    bool check(const Matrix &A, const Array &x, const Array &b, mreal eps = -1);
    extern mreal checker_eps;

    extern int max_iter;
    extern std::vector<Array> path;
};

#endif // LINEARSOLVEITER_H
