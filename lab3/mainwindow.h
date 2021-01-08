#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "linearsolve.h"
using namespace LinearSolve;
//#include <QTextDocument>
//#include <QTextBlock>
//#include <QMessageBox>
//#include <QDebug>
#include <QtPrintSupport/QtPrintSupport>

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

    void on_pushButtonGauss_clicked();

    void on_pushButtonGaussAd_clicked();

    void on_pushButtonSqrt_clicked();

    void on_pushButtonLDL_clicked();

    void on_pushButtonChase_clicked();

    void on_pushButtonLU_clicked();

    void on_pushButtonLL_clicked();

    void on_pushButtonLDL_2_clicked();

private:
    Ui::MainWindow *ui;

    bool check(const Matrix& A, const Array& b);
    QVector<QVector<double>> doc2vec(QTextDocument* doc);
    Matrix inputMatrix(QTextDocument* doc);
    bool inputChaseData(Array& lrst, Array& crst, Array& rrst, Array& brst);

};
#endif // MAINWINDOW_H
