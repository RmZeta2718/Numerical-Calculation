#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <algorithm>
#include "qcustomplot.h"
#include "interpolation.h"

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
    void on_HintBtn_clicked();

    void on_ComfirmBtn_clicked();

    void on_LagrangeBtn_clicked();

    void on_NewtonBtn_clicked();

    void on_SegmentBtn_clicked();

    void on_ShowAllBtn_clicked();

    void on_QueryBtn_clicked();

    void drawFunc();

private:
    Ui::MainWindow *ui;
    const int plot_cnt;
    const double stdInitPercent;

    void zhuangbility();
    void setConfirmText();
    void inputMatrix(QTextDocument* doc);
};

#endif // MAINWINDOW_H
