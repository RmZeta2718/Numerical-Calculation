#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "integration.h"
using namespace integration;

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
    void drawFunc();

    void on_radioButton_1_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_buttonT_clicked();

    void on_buttonS_clicked();

    void on_buttonST_clicked();

    void on_buttonSS_clicked();

    void on_buttonR_clicked();

private:
    Ui::MainWindow *ui;
    FuncType *func;
    double left_end, right_end, int_ans;

    void init_func(FuncType newfunc);
    void set_hint_text(bool have_bound);
    void update_info();
    void fit_bound_range();
    void zhuangbility();
    static QVector<double> linspace(double a, double b, int cnt);
};
#endif // MAINWINDOW_H
