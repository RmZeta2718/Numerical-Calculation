#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
                "第一个空行之后的输入都将被忽略\n"
                "============================\n"
                "若选择追赶法，可以使用矩阵输入，也可以只输入非零值\n");
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
    QString A = " 4 -1\n"
                "-1  4 -1\n"
                "-1  4 -1\n"
                "-1  4 -1\n"
                "-1  4 -1\n"
                "-1  4 -1\n"
                "-1  4 -1\n"
                "-1  4 -1\n"
                "-1  4 -1\n"
                "-1  4\n"
                "\n"
                "======以下为注释======\n"
                "上述输入对应的矩阵为：\n"
                " 4 -1  0  0  0  0  0  0  0  0\n"
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

bool MainWindow::inputChaseData(Array &lrst, Array &crst, Array &rrst, Array &brst)
{
    Array b = inputMatrix(ui->textEditInput_2->document());
    auto vec = doc2vec(ui->textEditInput_1->document());
    if (b.row != vec.size() || b.col != 1)
        return false;
    if (vec.front().size() != 2 || vec.back().size() != 2)
        return false;
    for (int i = 1; i < vec.size() - 1; ++i)
        if (vec[i].size() != 3)
            return false;
    Array l(b.row - 1, 1), c(b.row, 1), r(b.row - 1, 1);
    c(0) = vec[0][0];
    r(0) = vec[0][1];
    for (int i = 1; i < vec.size() - 1; ++i) {
        l(i - 1) = vec[i][0];
        c(i) = vec[i][1];
        r(i) = vec[i][2];
    }
    l(b.row - 2) = vec.back()[0];
    c(b.row - 1) = vec.back()[1];
    lrst = l;
    crst = c;
    rrst = r;
    brst = b;
    return true;
}

void MainWindow::on_pushButtonGauss_clicked()
{
    Matrix A = inputMatrix(ui->textEditInput_1->document());
    Array b = inputMatrix(ui->textEditInput_2->document());
    if (!check(A, b)) return;
    Array x = GaussSolve(A, b);
    if (x.empty()) {
        ui->textEditOutput->setText("解不存在或不唯一");
        return;
    }
    ui->textEditOutput->setText(x.to_string().c_str());

    if (!LinearSolve::check(A, x, b))
        QMessageBox::warning(this, "警告", "检测到结果误差较大\n请选择更合适的算法");
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

void MainWindow::on_pushButtonGaussAd_clicked()
{
    Matrix A = inputMatrix(ui->textEditInput_1->document());
    Array b = inputMatrix(ui->textEditInput_2->document());
    if (!check(A, b)) return;
    Array x = AdvancedGaussSolve(A, b);
    if (x.empty()) {
        ui->textEditOutput->setText("解不存在或不唯一");
        return;
    }
    ui->textEditOutput->setText(x.to_string().c_str());

    if (!LinearSolve::check(A, x, b))
        QMessageBox::warning(this, "警告", "检测到结果误差较大\n请选择更合适的算法");
}

void MainWindow::on_pushButtonSqrt_clicked()
{
    Matrix A = inputMatrix(ui->textEditInput_1->document());
    Array b = inputMatrix(ui->textEditInput_2->document());
    if (!check(A, b)) return;
    Array x = SquareRootSolve(A, b);
    ui->textEditOutput->setText(x.to_string().c_str());

    if (!LinearSolve::check(A, x, b))
        QMessageBox::warning(this, "警告", "检测到结果误差较大\n请选择更合适的算法");
}

void MainWindow::on_pushButtonLDL_clicked()
{
    Matrix A = inputMatrix(ui->textEditInput_1->document());
    Array b = inputMatrix(ui->textEditInput_2->document());
    if (!check(A, b)) return;
    Array x = AdvancedSquareRootSolve(A, b);
    ui->textEditOutput->setText(x.to_string().c_str());

    if (!LinearSolve::check(A, x, b))
        QMessageBox::warning(this, "警告", "检测到结果误差较大\n请选择更合适的算法");
}

void MainWindow::on_pushButtonChase_clicked()
{
    Matrix A;
    Array b, l, c, r;
    QString msg = "是否通过矩阵输入？\n"
                  "选择是，则输入方式与其他按钮相同\n"
                  "选择否，则只输入矩阵中的非零值，排列次序与矩阵中的次序相同";
    auto choice = QMessageBox::question(this, "请选择输入格式", msg,
                         QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                         QMessageBox::Cancel);
    if (choice == QMessageBox::Yes) {
        A = inputMatrix(ui->textEditInput_1->document());
        b = inputMatrix(ui->textEditInput_2->document());
        if (!check(A, b)) return;

        l = Matrix(A.row - 1, 1);
        c = Matrix(A.row, 1);
        r = Matrix(A.row - 1, 1);
        c(0) = A(0, 0);
        r(0) = A(0, 1);
        for (int i = 1; i < A.row - 1; ++i) {
            l(i - 1) = A(i, i - 1);
            c(i) = A(i, i);
            r(i) = A(i, i + 1);
        }
        l(A.row - 2) = A(A.row - 1, A.row - 2);
        c(A.row - 1) = A(A.row - 1, A.row - 1);
    } else if (choice == QMessageBox::No) {
        if (!inputChaseData(l, c, r, b)) {
            QMessageBox::critical(this, "错误", "格式错误，请参照样例3");
            return;
        }
    } else return;
    Array x = ChaseSolve(l, c, r, b);
    ui->textEditOutput->setText(x.to_string().c_str());

    if (!LinearSolve::check(l, c, r, x, b))
        QMessageBox::warning(this, "警告", "检测到结果误差较大\n请选择更合适的算法");
}

void MainWindow::on_pushButtonLU_clicked()
{
    Matrix A = inputMatrix(ui->textEditInput_1->document());
    if (A.empty() || A.row != A.col) {
        QMessageBox::critical(this, "错误", "请正确输入矩阵A");
        return;
    }
    auto p = LUDecomposition(A);
    std::string msg = "L:\n" + p.first.to_string() + "\nU:\n" + p.second.to_string();
    ui->textEditOutput->setText(msg.c_str());
}

void MainWindow::on_pushButtonLL_clicked()
{
    Matrix A = inputMatrix(ui->textEditInput_1->document());
    if (A.empty() || A.row != A.col) {
        QMessageBox::critical(this, "错误", "请正确输入矩阵A");
        return;
    }
    Matrix L = LLTDecomposition(A);
    std::string msg = "L:\n" + L.to_string();
    ui->textEditOutput->setText(msg.c_str());
}

void MainWindow::on_pushButtonLDL_2_clicked()
{
    Matrix A = inputMatrix(ui->textEditInput_1->document());
    if (A.empty() || A.row != A.col) {
        QMessageBox::critical(this, "错误", "请正确输入矩阵A");
        return;
    }
    auto p = LDLTDecomposition(A);
    std::string msg = "L:\n" + p.first.to_string() + "\nD（仅输出对角元）:\n" + p.second.to_string();
    ui->textEditOutput->setText(msg.c_str());
}
