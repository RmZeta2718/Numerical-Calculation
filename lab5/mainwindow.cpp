#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
using namespace std;
#define mydebug(x) cout << __FUNCTION__ << (": " #x "\n") << (x) << endl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , threshold(30)
    , expandRate(1.1)
{
    ui->setupUi(this);
    connect(ui->customPlot,SIGNAL(beforeReplot()),this,SLOT(showPoints()));
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("迭代次数");
    ui->customPlot->yAxis->setLabel("特征向量误差");
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    // init graphs
    ui->customPlot->addGraph();
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    ui->customPlot->graph(0)->setPen(pen);
    pen.setColor(Qt::blue);
    ui->customPlot->graph(1)->setPen(pen);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonInput_hint_clicked()
{
    QString msg("左侧框内输入矩阵，行用回车分隔，列可以用若干个空格或制表符（TAB）分隔\n"
                "右侧框内输入迭代初始向量，每行一个数字\n"
                "不需要输入维数，只要输入格式正确，就能自动计算维数\n"
                "第一个空行之后的输入都将被忽略\n");
    QMessageBox::information(this, "输入格式", msg);
}

void MainWindow::on_pushButtonInput_1_clicked()
{
    QString A = "-1   2   1\n"
                " 2  -4   1\n"
                " 1   1  -6\n";
    QString b = "1\n1\n1\n";
    ui->textEditInput_1->setPlainText(A);
    ui->textEditInput_2->setPlainText(b);
    ui->textEditOutput->setPlainText("");
}

void MainWindow::on_pushButtonInput_2_clicked()
{
    QString A = " 4 -2  7  3 -1  8\n"
                "-2  5  1  1  4  7\n"
                " 7  1  7  2  3  5\n"
                " 3  1  2  6  5  1\n"
                "-1  4  3  5  3  2\n"
                " 8  7  5  1  2  4\n";
    QString b = "1\n0\n1\n0\n0\n1\n";
    ui->textEditInput_1->setPlainText(A);
    ui->textEditInput_2->setPlainText(b);
    ui->textEditOutput->setPlainText("");
}

void MainWindow::on_pushButtonInput_3_clicked()
{
    QString A = " 2 -1  0  0  0\n"
                "-1  2 -1  0  0\n"
                " 0 -1  2 -1  0\n"
                " 0  0 -1  2 -1\n"
                " 0  0  0 -1  2\n";
    QString b = "1\n1\n1\n1\n1\n";
    ui->textEditInput_1->setPlainText(A);
    ui->textEditInput_2->setPlainText(b);
    ui->textEditOutput->setPlainText("");
}

void MainWindow::on_pushButtonInput_4_clicked()
{
    QString A = "2  1  3  4\n"
                "1 -3  1  5\n"
                "3  1  6 -2\n"
                "4  5 -2 -1\n";
    QString b = "1\n1\n1\n1\n";
    ui->textEditInput_1->setPlainText(A);
    ui->textEditInput_2->setPlainText(b);
    ui->textEditOutput->setPlainText("");
}

void MainWindow::on_pushButtonInput_5_clicked()
{
    QString A = "-1  2  1\n"
                " 2 -4  1\n"
                " 1  1 -6\n";
    QString b = "1\n1\n1\n";
    ui->textEditInput_1->setPlainText(A);
    ui->textEditInput_2->setPlainText(b);
    ui->textEditOutput->setPlainText("");
}

Matrix MainWindow::inputMatrix(QTextDocument *doc)
{
    auto mat = doc2vec(doc);
    if (!mat.size() || !mat[0].size())
        return Matrix();
    for (int i = 1; i < mat.size(); ++i)
        if (mat[i].size() != mat[0].size())
            return Matrix();

    Matrix rst(mat.size(), mat[0].size());
    for (int i = 0; i < mat.size(); ++i)
        for (int j = 0; j < mat[i].size(); ++j)
            rst(i, j) = mat[i][j];
    return rst;
}

bool MainWindow::check(const Matrix &A, const Array &b)
{
    if (A.empty() || A.row != A.col) {
        QMessageBox::critical(this, "错误", "请正确输入矩阵A");
        return false;
    }
    if (b.empty() || b.col != 1) {
        QMessageBox::critical(this, "错误", "请正确输入向量b");
        return false;
    }
    if (A.row != b.row) {
        QMessageBox::critical(this, "错误", "矩阵A与向量b的大小不匹配");
        return false;
    }
    return true;
}

QVector<QVector<double> > MainWindow::doc2vec(QTextDocument *doc)
{
    QVector<QVector<double>> rst;
    int tot = doc->blockCount();        //回车符分隔block
    for (int i = 0; i < tot; i++) {
        QString line = doc->findBlockByNumber(i).text();
        line = line.simplified();
        if (line.isEmpty()) break;

        rst.push_back(QVector<double>());
        bool ok = true;
        for (int idx = 0; ; ++idx) {
            double num = line.section(' ', idx, idx).toDouble(&ok);
            if (!ok) break;
            rst[i].push_back(num);
        }
    }
    return rst;
}

void MainWindow::on_pushButtonPower_clicked()
{
    Matrix A = inputMatrix(ui->textEditInput_1->document());
    Array b = inputMatrix(ui->textEditInput_2->document());
    if (!check(A, b)) return;
    double eps = ui->doubleSpinBox_eps->value();
    double p = ui->doubleSpinBox_p->value();
    auto rst = PowerMethod(A, b, p, eps);

    ui->textEditOutput->setText(QString("Eigen Value:\n%1\nx:").arg(rst.eigenValue));

    drawFunc(EigenValueIter::path, rst.eigenValue < 0);

    ui->textEditOutput->append(rst.eigenVector.to_string().c_str());

    if (!EigenValueIter::check(A, rst, eps * 10))
        QMessageBox::warning(this, "警告", "检测到结果误差较大\n请选择更合适的算法");
}

void MainWindow::on_pushButtonInversePower_clicked()
{
    Matrix A = inputMatrix(ui->textEditInput_1->document());
    Array b = inputMatrix(ui->textEditInput_2->document());
    if (!check(A, b)) return;
    double eps = ui->doubleSpinBox_eps->value();
    double p = ui->doubleSpinBox_p->value();
    auto rst = InversePowerMethod(A, b, p, eps);

    ui->textEditOutput->setText(QString("Eigen Value:\n%1\nx:").arg(rst.eigenValue));

    drawFunc(EigenValueIter::path, rst.eigenValue < 0);

    ui->textEditOutput->append(rst.eigenVector.to_string().c_str());

    if (!EigenValueIter::LUsuccess)
        QMessageBox::warning(this, "警告", "LU分解失败！\n正在使用高斯消元法");

    if (!EigenValueIter::check(A, rst, eps * 10))
        QMessageBox::warning(this, "警告", "检测到结果误差较大\n请选择更合适的算法");
}

void MainWindow::showPoints()
{
    auto range = ui->customPlot->xAxis->range();
    ui->customPlot->graph(0)->setVisible(range.upper - range.lower < threshold);
}

void MainWindow::drawFunc(const std::vector<Array> &path, bool neg)
{
    QVector<double> vx, vy;
    for (int i = 1; i < (int)path.size(); ++i) {
        vx.push_back(i);
//        mydebug(path[i].to_string())
        vy.push_back((path[i] - (neg ? -1.0 : 1.0) * path[i - 1]).norm_2());
    }

    ui->customPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    ui->customPlot->yAxis->setTicker(QSharedPointer<QCPAxisTickerLog>(new QCPAxisTickerLog));
//    ui->customPlot->yAxis->setScaleType(QCPAxis::stLinear);
//    ui->customPlot->yAxis->setTicker(QSharedPointer<QCPAxisTicker>(new QCPAxisTicker));

    ui->customPlot->graph(0)->setData(vx, vy);
    ui->customPlot->graph(1)->setData(vx, vy);
    ui->customPlot->graph(0)->setVisible(vx.size() < threshold);
//    ui->customPlot->graph(0)->setVisible(true);
//    ui->customPlot->graph(1)->setVisible(false);

    ui->customPlot->rescaleAxes();
    auto range = ui->customPlot->xAxis->range();
    ui->customPlot->xAxis->setRange(range.center(), range.size() * expandRate, Qt::AlignCenter);
    range = ui->customPlot->yAxis->range();
    double lower = log(range.lower), upper = log(range.upper), size = (upper - lower) * expandRate;
    lower = (upper + lower) / 2 - size / 2;
    ui->customPlot->yAxis->setRange(exp(lower), exp(lower + size), Qt::AlignLeft);
    ui->customPlot->replot();
}

