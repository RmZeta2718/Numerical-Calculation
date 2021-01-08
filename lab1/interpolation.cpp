#include "interpolation.h"
#include "QtDebug"

int Interpolate::segmentPower = 0;
QVector<QPointF> Interpolate::stdPoints;
QVector<double> diff4Newton;
QVector<QVector<double>> diffs4Segment;
QVector<QVector<QPointF>> points4Segment;

void Interpolate::Init(const QVector<QPointF>& stdPoints)
{
    // 初始化全局变量，牛顿法差分表
    Interpolate::stdPoints = stdPoints;
    diff4Newton = GenNewtonDiff(stdPoints);
}

double Interpolate::Lagrange(double x)
{
    // 拉格朗日插值
    double rst = 0, up, down, xi, xj, yi;
    for (int i = 0; i < stdPoints.size(); ++i) {
        up = 1;
        down = 1;
        xi = stdPoints[i].x();
        yi = stdPoints[i].y();
        for (int j = 0; j < stdPoints.size(); ++j) if (i != j) {
            xj = stdPoints[j].x();
            up *= x - xj;
            down *= xi - xj;
        }
        rst += yi * up / down;
    }
    return rst;
}

double Interpolate::Newton_Segment(double x, const QVector<QPointF> &points, const QVector<double> &diff)
{
    // 牛顿插值
    double rst = diff[0], Pai_x_minus_xi = 1;
    for (int i = 1; i < diff.size(); ++i) {
        Pai_x_minus_xi *= x - points[i - 1].x();
        rst += diff[i] * Pai_x_minus_xi;
    }
    return rst;
}

double Interpolate::Newton(double x)
{
    // 封装牛顿插值，对所有数据插值
    return Newton_Segment(x, stdPoints, diff4Newton);
}


QVector<double> Interpolate::GenNewtonDiff(const QVector<QPointF>& points)
{
    // 初始化差分表
    int n = points.size();
    const auto &p = points;
    QVector<double> tmp, rst;
    for (int i = 0; i < n; ++i)
        tmp.push_back(p[i].y());
    rst.push_back(tmp.front());
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < n - i; ++j)
            tmp[j] = (tmp[j + 1] - tmp[j]) / (p[j + i].x() - p[j].x());
        rst.push_back(tmp.front());
        //        qDebug() << "i:\t" << i;
        //        for (int j = 0; j < n - i; ++j)
        //            qDebug() << tmp[j];
    }
//    qDebug() << rst;
//    assert(rst.size() == n);
    return rst;
}

bool Interpolate::SegmentInit(int p)
{
    // 初始化分段差分表
    if (stdPoints.size() <= p)
        return false;
    points4Segment.clear();
    diffs4Segment.clear();
    segmentPower = p;
    int idx = 0;
    while (idx + p < stdPoints.size()) {
        QVector<QPointF> tmp(stdPoints.begin() + idx, stdPoints.begin() + idx + p + 1);
        points4Segment.push_back(tmp);
        diffs4Segment.push_back(GenNewtonDiff(tmp));
        idx += p;
    }
    if (idx != stdPoints.size() - 1) {
        QVector<QPointF> tmp(stdPoints.end() - p - 1, stdPoints.end());
        points4Segment.push_back(tmp);
        diffs4Segment.push_back(GenNewtonDiff(tmp));
    }
//    foreach (auto v, diffs4Segment)
//        assert(v.size() == p + 1);
    return true;
}

double Interpolate::Segment(double x)
{
    // 封装分段插值
    int pos = std::lower_bound(stdPoints.begin(), stdPoints.end(), QPointF(x, 0), QPointCmp) - stdPoints.begin();
    QVector<double> diff;
    QVector<QPointF> points;
//    qDebug() << pos;
    int idx;
    if (pos == 0) idx = 0;
    else if (pos == stdPoints.size()) idx = diffs4Segment.size() - 1;
    else idx = (pos - 1) / segmentPower;

    points = points4Segment[idx];
    diff = diffs4Segment[idx];
    return Newton_Segment(x, points, diff);
}

bool Interpolate::QPointCmp(const QPointF &p1, const QPointF &p2)
{
    // 用于排序
    return p1.x() < p2.x();
}

