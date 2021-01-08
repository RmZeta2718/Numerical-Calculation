#ifndef EIGENVALUEITER_H
#define EIGENVALUEITER_H

#include "matrix.h"

//#include <iostream>
//using namespace std;
//#define mydebug(x) cout << __FUNCTION__ << (": " #x "\n") << (x) << endl;


namespace EigenValueIter{
    // result structure
    struct EigenPair_t {
        mreal eigenValue;
        Array eigenVector;
    };

    // solvers
    EigenPair_t PowerMethod(Matrix A, Array init = Matrix(), mreal p = 0, mreal eps = 1e-6);
    EigenPair_t InversePowerMethod(Matrix A, Array init = Matrix(), mreal p = 0, mreal eps = 1e-6);

    // checkers
    bool check(const Matrix &A, const EigenPair_t &rst, mreal eps = -1);
    extern mreal checker_eps;

    extern int max_iter;
    extern bool LUsuccess;
    extern std::vector<Array> path;
};

#endif // EIGENVALUEITER_H
