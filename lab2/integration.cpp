#include "integration.h"
#include <QDebug>
#include <QString>
#include <QtMath>

double integration::Romberg(integration::FuncType func, double a, double b, double eps)
{
    static const int maxk = 31;
    double arr[maxk][2];
    arr[0][0] = T_n(func, a, b, 1);
    int k, cur;
    for (k = 1, cur = 1; k < maxk; ++k, cur = !cur) {
        // R_{k,0}
        arr[0][cur] = T_n(func, a, b, 1 << k);
        // R_{k,1} ~ R_{k, k}
        double pow_4 = 4;
        for (int j = 1; j <= k; ++j, pow_4 *= 4)
            arr[j][cur] = arr[j - 1][cur] + (arr[j - 1][cur] - arr[j - 1][!cur]) / (pow_4 - 1);
//        QString dbg = QString("k=%1").arg(k);
//        for (int j = 0; j <= k; ++j)
//            dbg += QString("R_k,%1=%2  ").arg(j).arg(arr[j][cur]);
//        qDebug() << dbg;
        if (qAbs(arr[k][cur] - arr[k - 1][!cur]) < eps) break;
    }
    return arr[k][cur];
}

double integration::T_n(integration::FuncType func, double a, double b, int n)
{
    double rst = (func(a) + func(b)) / 2, h = (b - a) / n, x = a;
    for (int _i = 1; _i < n; ++_i) {
        x += h;
        rst += func(x);
    }
    rst *= h;
    return rst;
}

double integration::Simpson_n(integration::FuncType func, double a, double b, int n)
{
    assert(!(n & 1));       // n has to be even
    double rst = func(a) + func(b), h = (b - a) / n, x;
    x = a + h;
    for (int _i = 0; _i < n / 2; ++_i, x += 2 * h)
        rst += 4 * func(x);
    x = a + 2 * h;
    for (int _i = 1; _i < n / 2; ++_i, x += 2 * h)
        rst += 2 * func(x);
    rst *= h / 3;
    return rst;
}

double integration::T_eps(integration::FuncType func, double a, double b, double eps, int init_n)
{
    double rst[2];
    int cur = 0, n = init_n;
    rst[cur] = T_n(func, a, b, n);
    do {
        n <<= 1;
        if (n < 0) break;       // overflow
        cur = !cur;
        rst[cur] = T_n(func, a, b, n);
    } while (qAbs(rst[0] - rst[1]) > eps);
    return rst[cur];
}

double integration::Simpson_eps(integration::FuncType func, double a, double b, double eps, int init_n)
{
    double rst[2];
    int cur = 0, n = init_n;
    rst[cur] = Simpson_n(func, a, b, n);
    do {
        n <<= 1;
        if (n < 0) break;       // overflow
        cur = !cur;
        rst[cur] = Simpson_n(func, a, b, n);
    } while (qAbs(rst[0] - rst[1]) > eps);
    return rst[cur];
}
