#include "mainwindow.h"
#include "ui_mainwindow.h"

double f1(double x) {
    return qSqrt(4.0 - qSin(x) * qSin(x));
}
double f2(double x) {
    if (qAbs(x) < 1e-10) return 1.0;
    return qSin(x) / x;
}
double f3(double x) {
    return qExp(x) / (4.0 + x * x);
}
double f4(double x) {
    if (x < -1 + 1e-10) return 0.0 / 0.0;
    return qLn(1.0 + x) / (1.0 + x * x);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , func(nullptr)
{
    ui->setupUi(this);

    // init plot module
    connect(ui->customPlot,SIGNAL(beforeReplot()),this,SLOT(drawFunc()));
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("f(x)");
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    // init graphs
    ui->customPlot->addGraph();
    ui->customPlot->addGraph();
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    ui->customPlot->graph(0)->setPen(pen);
    // paint integratal region
//    ui->customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(1)->setBrush(QBrush(QColor(0, 0, 255, 100)));
    ui->customPlot->graph(1)->setVisible(false);

    set_hint_text(false);
//    ui->customPlot->yAxis->setScaleRatio(ui->customPlot->xAxis);
//    auto xRange = ui->customPlot->xAxis->range();
//    qDebug() << xRange.size();
//    auto yRange = ui->customPlot->yAxis->range();
//    qDebug() << yRange.size();
//    auto sz = ui->customPlot->size();
//    qDebug() << sz;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawFunc()
{
    if (!func) return;
    static const int plot_cnt = 50000;
    QVector<double> vx, vy;
    // init vx
    auto xRange = ui->customPlot->xAxis->range();
    vx = linspace(xRange.lower, xRange.upper, plot_cnt);
    // calc vy
    foreach (double x, vx)
        vy.push_back(func(x));
    ui->customPlot->graph(0)->setData(vx, vy);
    if (ui->customPlot->graph(1)->visible()) {
        double left = qMax(left_end, xRange.lower), right = qMin(right_end, xRange.upper);
//        qDebug() << left;
//        qDebug() << right;
        vx = linspace(left, right, qCeil((right - left) / xRange.size() * plot_cnt));
        vy.clear();
        foreach (double x, vx)
            vy.push_back(func(x));
//        qDebug() << vx.size();
//        qDebug() << vy.size();
        ui->customPlot->graph(1)->setData(vx, vy);
    }
}

void MainWindow::on_radioButton_1_clicked()
{
    init_func(f1);
}

void MainWindow::on_radioButton_2_clicked()
{
    init_func(f2);
}

void MainWindow::on_radioButton_3_clicked()
{
    init_func(f3);
}

void MainWindow::on_radioButton_4_clicked()
{
    init_func(f4);
}

void MainWindow::init_func(FuncType newfunc)
{
    func = newfunc;
    ui->customPlot->graph(1)->setVisible(false);
    ui->customPlot->xAxis->setRange(0, 10, Qt::AlignCenter);
    ui->customPlot->yAxis->setRange(0, 10, Qt::AlignCenter);
    ui->customPlot->xAxis->setScaleRatio(ui->customPlot->yAxis);
    ui->customPlot->replot();
    set_hint_text(false);
    zhuangbility();
}

void MainWindow::set_hint_text(bool have_bound)
{
    QString txt("f(x)=");
    if (!func)
        txt += QStringLiteral("None\n");
    else if (func == f1)
        txt += QStringLiteral("Sqrt(4.0 - Sin(x) * Sin(x))\n");
    else if (func == f2)
        txt += QStringLiteral("Sin(x) / x\n");
    else if (func == f3)
        txt += QStringLiteral("Exp(x) / (4.0 + x * x)\n");
    else if (func == f4)
        txt += QStringLiteral("Ln(1.0 + x) / (1.0 + x * x)\n");
    if (have_bound) {
        txt += QString("下界=%1, 上界=%2, 答案=%3").
                arg(left_end).arg(right_end).arg(int_ans, 0, 'g', 10);
    }
    ui->textBrowser->setText(txt);

}

void MainWindow::update_info()
{
    set_hint_text(true);
    ui->customPlot->graph(1)->setVisible(true);
    fit_bound_range();
    ui->customPlot->replot();
}

void MainWindow::fit_bound_range()
{
    // 根据函数和积分区域调整绘图区域
    static const double ratio = 1.618;
    ui->customPlot->xAxis->setRange((left_end + right_end) / 2,
                                    (right_end - left_end) * ratio,
                                    Qt::AlignCenter);
    double ymin = std::min({0.0, func(left_end), func(right_end)});
    double ymax = std::max({0.0, func(left_end), func(right_end)});
    ui->customPlot->yAxis->setRange((ymin + ymax) / 2,
                                    (ymax - ymin) * ratio,
                                    Qt::AlignCenter);
    if (right_end - left_end > ymax - ymin)
        ui->customPlot->yAxis->setScaleRatio(ui->customPlot->xAxis);
    else
        ui->customPlot->xAxis->setScaleRatio(ui->customPlot->yAxis);
}

void MainWindow::zhuangbility()
{
    static bool first = true;
    if (first) {
         QMessageBox::information(this, "提示", "查看函数图像时，可以用鼠标拖动移动视角，滚轮缩放视角");
         first = false;
    }
}

QVector<double> MainWindow::linspace(double a, double b, int cnt)
{
    // generate *cnt* points in [a, b]
    QVector<double> rst;
    double step = (b - a) / (cnt - 1), x = a;
    for (int _i = 0; _i < cnt; ++_i, x += step)
        rst.push_back(x);
    return rst;
}

void MainWindow::on_buttonT_clicked()
{
    if (!func) {
        QMessageBox::critical(this, "错误", "请先选择被积函数（左上角）");
        return;
    }
    int default_val = 100, min_val = 10, max_val = INT_MAX, n;
    bool ok;
    n = QInputDialog::getInt(this, "复化梯形积分", "分段数n=",
                             default_val, min_val, max_val, 1, &ok);
    if (!ok) return;
    left_end = ui->doubleSpinBox_a->value();
    right_end = ui->doubleSpinBox_b->value();
    int_ans = T_n(func, left_end, right_end, n);
    update_info();
}

void MainWindow::on_buttonS_clicked()
{
    if (!func) {
        QMessageBox::critical(this, "错误", "请先选择被积函数（左上角）");
        return;
    }
    int default_val = 100, min_val = 10, max_val = INT_MAX, n;
    bool ok;
    n = QInputDialog::getInt(this, "复化辛普森积分", "分段数n=",
                             default_val, min_val, max_val, 1, &ok);
    if (!ok) return;
    if (n & 1) {
        QString msg("n=2m，必须是偶数\n");
        msg += QString("您输入了：%1").arg(n);
        QMessageBox::critical(this, "复化辛普森积分", msg);
        return;
    }
    left_end = ui->doubleSpinBox_a->value();
    right_end = ui->doubleSpinBox_b->value();
    int_ans = Simpson_n(func, left_end, right_end, n);
    update_info();
}

void MainWindow::on_buttonST_clicked()
{
    if (!func) {
        QMessageBox::critical(this, "错误", "请先选择被积函数（左上角）");
        return;
    }
    double default_val = 1e-4, min_val = 1e-10, max_val = 1e2, eps;
    bool ok;
    eps = QInputDialog::getDouble(this, "自适应梯形积分", "精度要求eps=",
                                  default_val, min_val, max_val, 10, &ok);
    if (!ok) return;
    left_end = ui->doubleSpinBox_a->value();
    right_end = ui->doubleSpinBox_b->value();
    int_ans = T_eps(func, left_end, right_end, eps);
    update_info();
}

void MainWindow::on_buttonSS_clicked()
{
    if (!func) {
        QMessageBox::critical(this, "错误", "请先选择被积函数（左上角）");
        return;
    }
    double default_val = 1e-4, min_val = 1e-10, max_val = 1e2, eps;
    bool ok;
    eps = QInputDialog::getDouble(this, "自适应辛普森积分", "精度要求eps=",
                                  default_val, min_val, max_val, 10, &ok);
    if (!ok) return;
    left_end = ui->doubleSpinBox_a->value();
    right_end = ui->doubleSpinBox_b->value();
    int_ans = Simpson_eps(func, left_end, right_end, eps);
    update_info();
}

void MainWindow::on_buttonR_clicked()
{
    if (!func) {
        QMessageBox::critical(this, "错误", "请先选择被积函数（左上角）");
        return;
    }
    double default_val = 1e-4, min_val = 1e-10, max_val = 1e2, eps;
    bool ok;
    eps = QInputDialog::getDouble(this, "龙贝格积分", "精度要求eps=",
                                  default_val, min_val, max_val, 10, &ok);
    if (!ok) return;
    left_end = ui->doubleSpinBox_a->value();
    right_end = ui->doubleSpinBox_b->value();
    int_ans = Romberg(func, left_end, right_end, eps);
    update_info();
}
