#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , plot_cnt(200)
    , stdInitPercent(0.6)
{
    ui->setupUi(this);
    connect(ui->customPlot,SIGNAL(beforeReplot()),this,SLOT(drawFunc()));
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("f(x)");
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    // init graphs
    ui->customPlot->addGraph();
    ui->customPlot->addGraph();
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setName("插值点");
    ui->customPlot->graph(1)->setName("拉格朗日/牛顿插值曲线");
    ui->customPlot->graph(2)->setName("分段插值曲线");
    ui->customPlot->graph(1)->setVisible(false);
    ui->customPlot->graph(2)->setVisible(false);
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    ui->customPlot->graph(0)->setPen(pen);
    pen.setColor(Qt::blue);
    ui->customPlot->graph(1)->setPen(pen);
    pen.setColor(Qt::red);
    ui->customPlot->graph(2)->setPen(pen);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setConfirmText()
{
    QString confirm_text = QString("您输入了 %1 个点：\n").arg(Interpolate::stdPoints.size());
    for (int i = 0; i < Interpolate::stdPoints.size(); ++i) {
        const QPointF& point = Interpolate::stdPoints[i];
        confirm_text += QString("x_%1=%2\tf(x_%3)=%4\n").arg(i).arg(point.x()).arg(i).arg(point.y());
    }
    confirm_text += "当前分段插值次数：";
    if (Interpolate::segmentPower) confirm_text += QString("%1次\n").arg(Interpolate::segmentPower);
    else confirm_text += "未设定";
    ui->textBrowser->setPlainText(confirm_text);
}

void MainWindow::on_HintBtn_clicked()
{
    const QString hint_text = "每行输入一个点的x和y，用空格或TAB分隔\n"
                              "输入完成后，请先点击导入，然后选择右侧按钮";
    QMessageBox::information(this, "输入格式", hint_text);
}

void MainWindow::on_ComfirmBtn_clicked()
{
    QVector<QPointF> inputPoints;
    //文本对象
    QTextDocument* doc = ui->plainTextEdit->document();
    int tot = doc->blockCount();        //回车符分隔block
    for (int i = 0; i < tot; i++) {
        QString line = doc->findBlockByNumber(i).text();
        line = line.simplified();
        if (line.isEmpty()) break;

        bool okx, oky;
        double x, y;
        x = line.section(' ', 0, 0).toDouble(&okx);
        y = line.section(' ', 1, 1).toDouble(&oky);

        // handle errors
        if (okx && oky) {
            inputPoints.push_back(QPointF(x, y));
        } else {
            QMessageBox::critical(this, "错误", "请输入实数");
            return;
        }
        if (!line.section(' ', 2, 2).isEmpty()) {
            QMessageBox::critical(this, "错误", "每行只能输入2个实数");
            return;
        }
    }
    std::sort(inputPoints.begin(), inputPoints.end(), Interpolate::QPointCmp);

    // init Newton Interpolation method
    Interpolate::Init(inputPoints);

    // draw interpolation points
    QVector<double> vx, vy;
    double xMin = inputPoints.front().x(), xMax = inputPoints.back().x();
    double yMin = inputPoints[0].y(), yMax = inputPoints[0].y();
    foreach (auto point, inputPoints) {
        vx.push_back(point.x());
        vy.push_back(point.y());
        yMin = qMin(yMin, point.y());
        yMax = qMax(yMin, point.y());
    }
    ui->customPlot->graph(0)->setData(vx, vy);

    // set init screen range
    double siz = qMax(xMax - xMin, yMax - yMin) / stdInitPercent;
    ui->customPlot->xAxis->setRange((xMin + xMax) / 2, siz, Qt::AlignCenter);
    ui->customPlot->yAxis->setRange((yMin + yMax) / 2, siz, Qt::AlignCenter);

    // clear segment method
    ui->customPlot->graph(2)->clearData();
    Interpolate::segmentPower = 0;
    ui->customPlot->replot();

    setConfirmText();
}


void MainWindow::on_LagrangeBtn_clicked()
{
    zhuangbility();
    ui->customPlot->graph(1)->setVisible(true);
    ui->customPlot->graph(2)->setVisible(false);
    ui->customPlot->replot();
}

void MainWindow::on_NewtonBtn_clicked()
{
    zhuangbility();
    ui->customPlot->graph(1)->setVisible(true);
    ui->customPlot->graph(2)->setVisible(false);
    ui->customPlot->replot();
}

void MainWindow::on_ShowAllBtn_clicked()
{
    zhuangbility();
    ui->customPlot->graph(1)->setVisible(true);
    ui->customPlot->graph(2)->setVisible(true);
    ui->customPlot->replot();
}

void MainWindow::on_SegmentBtn_clicked()
{
    zhuangbility();
    int default_val = 3, min_val = 1, max_val = 8, rst;
    bool ok;
    rst = QInputDialog::getInt(this, "分段n次插值", "插值次数n=",
                                        default_val, min_val, max_val, 1, &ok);
    if (!ok) return;
    if (!Interpolate::SegmentInit(rst)) {
        QMessageBox::critical(this, "错误",
            QString("无法在%1个点上做%2次插值").arg(Interpolate::stdPoints.size()).arg(rst));
    }
    ui->customPlot->graph(1)->setVisible(false);
    ui->customPlot->graph(2)->setVisible(true);
    ui->customPlot->replot();
    setConfirmText();
}

void MainWindow::drawFunc()
{
    if (Interpolate::stdPoints.empty()) return;

    QVector<double> vx, vy;
//     init vx
    auto xRange = ui->customPlot->xAxis->range();
    double left = xRange.lower, right = xRange.upper;
    double step = (right - left) / plot_cnt;
    for (double x = left; x < right; x += step)
        vx.push_back(x);
//    qDebug() << vx;

//     Lagrange is the same as Newton, but slower
//    vy.clear();
//    foreach (double x, vx)
//        vy.push_back(Interpolate::Lagrange(x));
//    ui->customPlot->graph(1)->setData(vx, vy);

//     Newton
    foreach (double x, vx)
        vy.push_back(Interpolate::Newton(x));
    ui->customPlot->graph(1)->setData(vx, vy);

//    segment
    if (!Interpolate::segmentPower) return;
    vy.clear();
    foreach (double x, vx)
        vy.push_back(Interpolate::Segment(x));
    ui->customPlot->graph(2)->setData(vx, vy);
}

void MainWindow::zhuangbility()
{
    static bool first = true;
    if (first) {
         QMessageBox::information(this, "提示", "查看插值曲线时，可以用鼠标拖动移动视角，滚轮缩放视角");
         first = false;
    }
}


void MainWindow::on_QueryBtn_clicked()
{
    double default_val = 0, min_val = -1e18, max_val = 1e18, x;
    bool ok;
    x = QInputDialog::getDouble(this, "单点查询", "x=",
                                default_val, min_val, max_val, 6, &ok);
    if (!ok) return;
    QString ans = QString("x=%1\n\n").arg(x);
    ans += QString("拉格朗日插值：\tf(x)=%1\n").arg(Interpolate::Lagrange(x));
    ans += QString("牛顿插值：\tf(x)=%1\n").arg(Interpolate::Newton(x));
    if (Interpolate::segmentPower) {
        ans += QString("分段%1次插值：\tf(x)=%2\n").
                arg(Interpolate::segmentPower).
                arg(Interpolate::Segment(x));
    } else {
        QMessageBox::warning(this, "警告", "若要查看分段n次插值结果，请先设置次数\n（点击上方分段n次插值按钮）");
        ans += "分段0次插值：\tf(x)=???\n";
    }
    QMessageBox::information(this, "单点插值结果", ans);
}
