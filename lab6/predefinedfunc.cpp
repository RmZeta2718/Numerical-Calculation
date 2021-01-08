#include "predefinedfunc.h"
#include <cmath>

#define UNUSED(x) (void)(x)

ndarray f1(real_type x, const ndarray &y) {
    ndarray rst(y.size());
    rst[0] = 4 * x / y[0] - x * y[0];
    return rst;
}

ndarray f2(real_type x, const ndarray &y) {
    ndarray rst(y.size());
    rst[0] = x * x - y[0] * y[0];
    return rst;
}

ndarray f3(real_type x, const ndarray &y) {
    ndarray rst(y.size());
    UNUSED(x);
    rst[0] = y[1];
    rst[1] = -y[0];
    rst[2] = -y[2];
    return rst;
}

ndarray f4(real_type x, const ndarray &y) {
    ndarray rst(y.size());
    UNUSED(x);
    rst[0] = y[1];
    rst[1] = 3 * y[1] - 2 * y[0];
    return rst;
}

ndarray f5(real_type x, const ndarray &y) {
    ndarray rst(y.size());
    UNUSED(x);
    rst[0] = y[1];
    rst[1] = 0.1 * (1 - y[0] * y[0]) * y[1] - y[0];
    return rst;
}

ndarray f6(real_type x, const ndarray &y) {
    ndarray rst(y.size());
    UNUSED(x);
    rst[0] = y[1];
    rst[1] = y[0] / (exp(x) + 1);
    return rst;
}

ndarray f7(real_type x, const ndarray &y) {
    ndarray rst(y.size());
    UNUSED(x);
    rst[0] = y[1];
    rst[1] = -sin(y[0]);
    return rst;
}
