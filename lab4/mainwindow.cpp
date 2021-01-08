#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    ui->customPlot->yAxis->setLabel("||x||_2");
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
                "右侧框内输入向量b，每行一个数字\n"
                "不需要输入维数，只要输入格式正确，就能自动计算维数\n"
                "第一个空行之后的输入都将被忽略\n");
    QMessageBox::information(this, "输入格式", msg);
}

void MainWindow::on_pushButtonInput_1_clicked()
{
    QString A = " 4  2  -3 -1  2   1  0  0  0  0\n"
                " 8  6  -5 -3  6   5  0  1  0  0\n"
                " 4  2  -2 -1  3   2 -1  0  3  1\n"
                " 0 -2   1  5 -1   3 -1  1  9  4\n"
                "-4  2   6 -1  6   7 -3  3  2  3\n"
                " 8  6  -8  5  7  17  2  6 -3  5\n"
                " 0  2  -1  3 -4   2  5  3  0  1\n"
                "16 10 -11 -9 17  34  2 -1  2  2\n"
                " 4  6   2 -7 13   9  2  0 12  4\n"
                " 0  0  -1  8 -3 -24 -8  6  3 -1\n";
    QString b = "5\n12\n3\n2\n3\n46\n13\n38\n19\n-21";
    ui->textEditInput_1->setPlainText(A);
    ui->textEditInput_2->setPlainText(b);
    ui->textEditOutput->setPlainText("");
}

void MainWindow::on_pushButtonInput_2_clicked()
{
    QString A = " 4  2 -4  0   2  4   0  0\n"
                " 2  2 -1 -2   1  3   2  0\n"
                "-4 -1 14  1  -8 -3   5  6\n"
                " 0 -2  1  6  -1 -4  -3  3\n"
                " 2  1 -8 -1  22  4 -10 -3\n"
                " 4  3 -3 -4   4 11   1 -4\n"
                " 0  2  5 -3 -10  1  14  2\n"
                " 0  0  6  3  -3 -4   2 19\n";
     QString b = "0\n-6\n20\n23\n9\n-22\n-15\n45";
     ui->textEditInput_1->setPlainText(A);
     ui->textEditInput_2->setPlainText(b);
     ui->textEditOutput->setPlainText("");
}

void MainWindow::on_pushButtonInput_3_clicked()
{
    QString A = " 4 -1  0  0  0  0  0  0  0  0\n"
                "-1  4 -1  0  0  0  0  0  0  0\n"
                " 0 -1  4 -1  0  0  0  0  0  0\n"
                " 0  0 -1  4 -1  0  0  0  0  0\n"
                " 0  0  0 -1  4 -1  0  0  0  0\n"
                " 0  0  0  0 -1  4 -1  0  0  0\n"
                " 0  0  0  0  0 -1  4 -1  0  0\n"
                " 0  0  0  0  0  0 -1  4 -1  0\n"
                " 0  0  0  0  0  0  0 -1  4 -1\n"
                " 0  0  0  0  0  0  0  0 -1  4\n";
    QString b = "7\n5\n-13\n2\n6\n-12\n14\n-4\n5\n-5";
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

void MainWindow::on_pushButtonJacobi_clicked()
{
    Matrix A = inputMatrix(ui->textEditInput_1->document());
    Array b = inputMatrix(ui->textEditInput_2->document());
    if (!check(A, b)) return;
    double eps = ui->doubleSpinBox->value();
    Array x = JacobiSolve(A, b, eps);

    ui->textEditOutput->setText("x:");

    drawFunc(LinearSolveIter::path);

    if (x.empty()) {
        ui->textEditOutput->append("发散");
        QMessageBox::information(this, "警告", "迭代发散！\n请选择更合适的算法");
        return;
    }

    ui->textEditOutput->append(x.to_string().c_str());

    if (!LinearSolveIter::check(A, x, b, eps * 10))
        QMessageBox::warning(this, "警告", "检测到结果误差较大\n请选择更合适的算法");
}

void MainWindow::on_pushButtonGauss_clicked()
{
    Matrix A = inputMatrix(ui->textEditInput_1->document());
    Array b = inputMatrix(ui->textEditInput_2->document());
    if (!check(A, b)) return;
    double eps = ui->doubleSpinBox->value();
    Array x = GaussSeidelSolve(A, b, eps);

    ui->textEditOutput->setText("x:");

    drawFunc(LinearSolveIter::path);

    if (x.empty()) {
        ui->textEditOutput->append("发散");
        QMessageBox::information(this, "警告", "迭代发散！\n请选择更合适的算法");
        return;
    }

    ui->textEditOutput->append(x.to_string().c_str());

    if (!LinearSolveIter::check(A, x, b, eps * 10))
        QMessageBox::warning(this, "警告", "检测到结果误差较大\n请选择更合适的算法");
}


void MainWindow::on_pushButtonSOR_clicked()
{
    Matrix A = inputMatrix(ui->textEditInput_1->document());
    Array b = inputMatrix(ui->textEditInput_2->document());
    if (!check(A, b)) return;

    double eps = ui->doubleSpinBox->value();

    double default_val = 0.8, min_val = 0, max_val = 1e18, omega;
    bool ok;
    omega = QInputDialog::getDouble(this, "SOR方法", "w=",
                                default_val, min_val, max_val, 6, &ok);
    if (!ok) return;

    ui->textEditOutput->setText(QString("w = %1\nx:").arg(omega));

    Array x = SORSolve(A, b, omega, eps);

    drawFunc(LinearSolveIter::path);

    if (x.empty()) {
        ui->textEditOutput->append("发散");
        QMessageBox::information(this, "警告", "迭代发散！\n请选择更合适的算法");
        return;
    }

    ui->textEditOutput->append(x.to_string().c_str());

    if (!LinearSolveIter::check(A, x, b, eps * 10))
        QMessageBox::warning(this, "警告", "检测到结果误差较大\n请选择更合适的算法");
}

void MainWindow::showPoints()
{
    auto range = ui->customPlot->xAxis->range();
    ui->customPlot->graph(0)->setVisible(range.upper - range.lower < threshold);
//    ui->customPlot->graph(0)->setVisible(true);
//    ui->customPlot->graph(1)->setVisible(false);
}

void MainWindow::drawFunc(const std::vector<Array> &path)
{
    bool diverge = false;
    QVector<double> vx, vy;
    for (int i = 0; i < (int)path.size(); ++i) {
        double norm = path[i].norm_2();
        if (norm > 1e100) {
            diverge = true;
            break;
        }
        vx.push_back(i);
        vy.push_back(norm);
    }

    if (diverge) {
        ui->customPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
        ui->customPlot->yAxis->setTicker(QSharedPointer<QCPAxisTickerLog>(new QCPAxisTickerLog));
    } else {
        ui->customPlot->yAxis->setScaleType(QCPAxis::stLinear);
        ui->customPlot->yAxis->setTicker(QSharedPointer<QCPAxisTicker>(new QCPAxisTicker));
    }

    ui->customPlot->graph(0)->setData(vx, vy);
    ui->customPlot->graph(1)->setData(vx, vy);
    ui->customPlot->graph(0)->setVisible(vx.size() < threshold);
//    ui->customPlot->graph(0)->setVisible(true);
//    ui->customPlot->graph(1)->setVisible(false);

    ui->customPlot->rescaleAxes();
    auto range = ui->customPlot->xAxis->range();
    ui->customPlot->xAxis->setRange(range.center(), range.size() * expandRate, Qt::AlignCenter);
    range = ui->customPlot->yAxis->range();
    ui->customPlot->yAxis->setRange(range.center(), range.size() * expandRate, Qt::AlignCenter);
    ui->customPlot->replot();
}
