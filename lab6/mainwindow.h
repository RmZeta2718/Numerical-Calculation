#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "odesolver.h"
using namespace OdeSolver;
#include "runtimedll.h"
using namespace RuntimeDll;

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

    void on_pushButtonInput_4_clicked();

    void on_pushButtonInput_5_clicked();

    void on_pushButtonInput_6_clicked();

    void on_pushButtonInput_7_clicked();

    void on_pushButtonCompile_clicked();

    void showPoints();

    void on_pushButtonEuler_clicked();

    void on_pushButtonEulerAdvanced_clicked();

    void on_pushButtonAdams_clicked();

    void on_pushButtonRK_clicked();

private:
    Ui::MainWindow *ui;
    int threshold;
    double expandRate;
    func_t func;
    point init;
    double last_h;
    HINSTANCE hdll;
    QString func_str;

    void on_pushButton_clicked(const QString &button_str);
    void drawFunc(const std::vector<point> &path);
    func_t compileFuncOnWindows();
    func_t getCustomFunc();
    QVector<QString> doc2vec(QTextDocument *doc);
    QVector<double> vs2vd(const QVector<QString> &vec);

};
#endif // MAINWINDOW_H
