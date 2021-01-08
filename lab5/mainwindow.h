#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "eigenvalueiter.h"
using namespace EigenValueIter;

#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonInput_hint_clicked();

    void on_pushButtonInput_1_clicked();

    void on_pushButtonInput_2_clicked();

    void on_pushButtonInput_3_clicked();

    void on_pushButtonPower_clicked();

    void on_pushButtonInversePower_clicked();

    void showPoints();

    void on_pushButtonInput_4_clicked();

    void on_pushButtonInput_5_clicked();

private:
    Ui::MainWindow *ui;
    int threshold;
    double expandRate;

    bool check(const Matrix& A, const Array& b);
    QVector<QVector<double>> doc2vec(QTextDocument* doc);
    Matrix inputMatrix(QTextDocument* doc);
    void drawFunc(const std::vector<Array> &path, bool neg);

};
#endif // MAINWINDOW_H
