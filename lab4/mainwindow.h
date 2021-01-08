#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "linearsolveiter.h"
using namespace LinearSolveIter;

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

    void on_pushButtonJacobi_clicked();

    void on_pushButtonGauss_clicked();

    void on_pushButtonSOR_clicked();

    void showPoints();

private:
    Ui::MainWindow *ui;
    int threshold;
    double expandRate;

    bool check(const Matrix& A, const Array& b);
    QVector<QVector<double>> doc2vec(QTextDocument* doc);
    Matrix inputMatrix(QTextDocument* doc);
    void drawFunc(const std::vector<Array> &path);

};
#endif // MAINWINDOW_H
