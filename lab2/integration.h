#ifndef INTEGRATION_H
#define INTEGRATION_H


namespace integration {
    typedef double FuncType(double);
    // 龙贝格积分
    double Romberg(FuncType func, double a, double b, double eps=1e-4);
    // 复化梯形
    double T_n(FuncType func, double a, double b, int n=10);
    // 复化辛普森
    double Simpson_n(FuncType func, double a, double b, int n=10);
    // 自适应梯形
    double T_eps(FuncType func, double a, double b, double eps=1e-4, int init_n=10);
    // 自适应辛普森
    double Simpson_eps(FuncType func, double a, double b, double eps=1e-4, int init_n=10);
}

#endif // INTEGRATION_H
