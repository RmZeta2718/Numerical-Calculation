#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <QVector>
#include <QPointF>

namespace Interpolate {
    extern int segmentPower;
    extern QVector<QPointF> stdPoints;
    void Init(const QVector<QPointF>& stdPoints);
    double Lagrange(double x);
    double Newton(double x);
    double Newton_Segment(double x, const QVector<QPointF>& points, const QVector<double>& diff);
    QVector<double> GenNewtonDiff(const QVector<QPointF>& points);
    bool SegmentInit(int p);
    double Segment(double x);
    bool QPointCmp(const QPointF& p1, const QPointF& p2);
}


#endif // INTERPOLATION_H
