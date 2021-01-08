#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "predefinedfunc.h"
//#include <iostream>
//using namespace std;
//#define mydebug(x) cout << __FUNCTION__ << (": " #x "\n") << (x) << endl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , threshold(30)
    , expandRate(1.1)
    , func(nullptr)
    , hdll(NULL)
    , func_str("")
{
    ui->setupUi(this);
    connect(ui->customPlot,SIGNAL(beforeReplot()),this,SLOT(showPoints()));
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y[0]");
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
    QString msg("左侧框内输入微分方程（组），右侧框内输入初值\n"
                "若要求解高阶微分方程，需要自己转化为1阶微分方程组\n"
                "推荐将y[0]作为原微分方程中的y，因为图像只包含y[0]\n"
                "不需要输入微分方程组个数，只要输入格式正确，就能自动计算\n"
                "第一个空行之后的输入都将被忽略\n"
                "输入格式与scipy等现代数值计算接口基本一致，如果你不熟悉，下面是简介：\n\n"
                "微分方程组是标量x和向量y的函数，向量y的每个分量都是一个因变量\n"
                "输入的微分方程（组）应当是一个合法的C++表达式\n"
                "第i行表示y[i]的导数，即只需要输入等号右边（i从0开始）\n\n"
                "初值第一行是x，之后每行分别是y[i]在x处的导数\n");
    QMessageBox::information(this, "输入格式", msg);
}

void MainWindow::on_pushButtonInput_1_clicked()
{
    QString f_str = "4 * x / y[0] - x * y[0]\n";
    QString init_str = "0.0000001\n"
                       "0.0000002\n";
    ui->textEditInput_1->setPlainText(f_str);
    ui->textEditInput_2->setPlainText(init_str);
    func = f1;
    ui->doubleSpinBox_h->setValue(0.1);
    ui->doubleSpinBox_x->setValue(2);
    func_str = f_str;
}

void MainWindow::on_pushButtonInput_2_clicked()
{
    QString f_str = "x * x - y[0] * y[0]\n";
    QString init_str = "-1\n"
                       "0\n";
    ui->textEditInput_1->setPlainText(f_str);
    ui->textEditInput_2->setPlainText(init_str);
    func = f2;
    ui->doubleSpinBox_h->setValue(0.01);
    ui->doubleSpinBox_x->setValue(0);
    func_str = f_str;
}

void MainWindow::on_pushButtonInput_3_clicked()
{
    QString f_str = "y[1]\n"
                    "-y[0]\n"
                    "-y[2]\n";
    QString init_str = "0\n"
                       "-1\n"
                       "0\n"
                       "1\n";
    ui->textEditInput_1->setPlainText(f_str);
    ui->textEditInput_2->setPlainText(init_str);
    func = f3;
    ui->doubleSpinBox_h->setValue(0.01);
    ui->doubleSpinBox_x->setValue(1);
    func_str = f_str;
}

void MainWindow::on_pushButtonInput_4_clicked()
{
    QString f_str = "y[1]\n"
                     "3 * y[1] - 2 * y[0]\n";
    QString init_str = "0\n"
                       "0\n"
                       "1\n";
    ui->textEditInput_1->setPlainText(f_str);
    ui->textEditInput_2->setPlainText(init_str);
    func = f4;
    ui->doubleSpinBox_h->setValue(0.02);
    ui->doubleSpinBox_x->setValue(1);
    func_str = f_str;
}

void MainWindow::on_pushButtonInput_5_clicked()
{
    QString f_str = "y[1]\n"
                     "0.1 * (1 - y[0] * y[0]) * y[1] - y[0]\n";
    QString init_str = "0\n"
                       "1\n"
                       "0\n";
    ui->textEditInput_1->setPlainText(f_str);
    ui->textEditInput_2->setPlainText(init_str);
    func = f5;
    ui->doubleSpinBox_h->setValue(0.1);
    ui->doubleSpinBox_x->setValue(1);
    func_str = f_str;
}

void MainWindow::on_pushButtonInput_6_clicked()
{
    QString f_str = "y[1]\n"
                     "y[0] / (exp(x) + 1)\n";
    QString init_str = "0\n"
                       "1\n"
                       "0\n";
    ui->textEditInput_1->setPlainText(f_str);
    ui->textEditInput_2->setPlainText(init_str);
    func = f6;
    ui->doubleSpinBox_h->setValue(0.1);
    ui->doubleSpinBox_x->setValue(2);
    func_str = f_str;
}

void MainWindow::on_pushButtonInput_7_clicked()
{
    QString f_str = "y[1]\n"
                     "-sin(y[0])\n";
    QString init_str = "0\n"
                       "1\n"
                       "0\n";
    ui->textEditInput_1->setPlainText(f_str);
    ui->textEditInput_2->setPlainText(init_str);
    func = f7;
    ui->doubleSpinBox_h->setValue(0.2);
    ui->doubleSpinBox_x->setValue(4);
    func_str = f_str;
}

void MainWindow::on_pushButtonCompile_clicked()
{
    func_t f = getCustomFunc();
    func = f;
    if (!func)
        return;
    func_str = ui->textEditInput_1->toPlainText();
    QMessageBox::information(this, "编译自定义函数", "编译成功！\n\n请选择算法");
}

void MainWindow::on_pushButtonEuler_clicked()
{
    on_pushButton_clicked("Euler");
}

void MainWindow::on_pushButtonEulerAdvanced_clicked()
{
    on_pushButton_clicked("EulerAd");
}

void MainWindow::on_pushButtonAdams_clicked()
{
    on_pushButton_clicked("Adams");
}

void MainWindow::on_pushButtonRK_clicked()
{
    on_pushButton_clicked("RK");
}

void MainWindow::on_pushButton_clicked(const QString &button_str)
{
    // robust
    if (func_str != ui->textEditInput_1->toPlainText()) {
        auto ans = QMessageBox::question(this, "未编译的修改", "您修改了函数，是否重新编译？");
        if (ans == QMessageBox::Yes) {
            on_pushButtonCompile_clicked();
            if (!func)  // compile failed
                return;
        }
        else return;
    }
    if (!func) {
        QMessageBox::critical(this, "错误", "请先指定函数或编译自定义函数");
        return;
    }
    auto init_code = vs2vd(doc2vec(ui->textEditInput_2->document()));
    if (doc2vec(ui->textEditInput_1->document()).size() + 1 != init_code.size()) {
        QMessageBox::critical(this, "错误", "函数个数与初值维数不匹配\n");
        return;
    }
    // get params
    init = {init_code[0], ndarray(init_code.begin() + 1, init_code.end())};
    double h = ui->doubleSpinBox_h->value();
    double x_tar = ui->doubleSpinBox_x->value();
    if (x_tar <= init.x)
        h *= -1;
    point p;
    // choose different method
    if (button_str == "Euler") {
        p = EulerMethod(func, init, h, x_tar);
    } else if (button_str == "EulerAd") {
        p = EulerAdvanced(func, init, h, x_tar);
    } else if (button_str == "Adams") {
        int default_val = 3, min_val = 2, max_val = 4, q;
        bool ok;
        q = QInputDialog::getInt(this, "显式Adams法", "q=",
                                 default_val, min_val, max_val, 1, &ok);
        if (!ok) return;
        p = AdamsMethod(func, init, h, x_tar, q);
    } else if (button_str == "RK") {
        int default_val = 4, min_val = 2, max_val = 4, order;
        bool ok;
        order = QInputDialog::getInt(this, "Runge-Kutta法", "order=",
                                     default_val, min_val, max_val, 1, &ok);
        if (!ok) return;
        p = RungeKuttaMethod(func, init, h, x_tar, order);
    }

    last_h = h;
    drawFunc(OdeSolver::path);

    if ((init.x < x_tar && p.x < x_tar) || (init.x > x_tar && p.x > x_tar))
        QMessageBox::warning(this, "警告", "迭代次数超过上限，请调整参数");
}

void MainWindow::showPoints()
{
    auto range = ui->customPlot->xAxis->range();
    ui->customPlot->graph(0)->setVisible((range.upper - range.lower) / last_h < threshold);
}

void MainWindow::drawFunc(const std::vector<point> &path)
{
    QVector<double> vx, vy;
    for (int i = 0; i < (int)path.size(); ++i) {
        vx.push_back(path[i].x);
        vy.push_back(path[i].y[0]);
//        mydebug(path[i].x)
//        for (int j = 0; j < (int)path[i].y.size(); ++j)
//            mydebug(path[i].y[j])
    }

    ui->customPlot->graph(0)->setData(vx, vy);
    ui->customPlot->graph(1)->setData(vx, vy);
    ui->customPlot->graph(0)->setVisible(vx.size() < threshold);

    ui->customPlot->rescaleAxes();
    auto range = ui->customPlot->xAxis->range();
    ui->customPlot->xAxis->setRange(range.center(), range.size() * expandRate, Qt::AlignCenter);
    range = ui->customPlot->yAxis->range();
    ui->customPlot->yAxis->setRange(range.center(), range.size() * expandRate, Qt::AlignCenter);
    ui->customPlot->replot();
}

func_t MainWindow::compileFuncOnWindows()
{
    static std::string dllname("dllfunc");
    QFile file(QString(dllname.c_str()) + ".cpp");
    if (!file.exists()) {
        QMessageBox::critical(this, "编译自定义函数", "源文件不存在！\n"
                                    "请检查dllfunc.cpp的位置\n"
                                    "可能是内部错误，请联系作者");
        return nullptr;
    }
    if (hdll) FreeLibrary(hdll);
    switch (compileDll(dllname)) {
    case COMMAND_NOT_EXEC:
        QMessageBox::critical(this, "编译自定义函数", "编译失败！\n"
                                    "可能是没有编译器，请确保环境中存在g++");
        return nullptr;
    case COMPILE_FAIL:
        QMessageBox::critical(this, "编译自定义函数", "编译失败！\n"
                                    "可能是函数不符合C++语法");
        return nullptr;
    }

    auto hinst = getDll(dllname);
    if (!hinst) {
        QMessageBox::critical(this, "编译自定义函数", "无法打开DLL文件！");
//        mydebug(GetLastError())
        return nullptr;
    }
    hdll = hinst;
    func_t f = (func_t)getFirstFuncInDll(hdll);
    if (!f) {
        QMessageBox::critical(this, "编译自定义函数", "无法找到目标函数！");
        return nullptr;
    }
    return f;
}

QVector<QString> MainWindow::doc2vec(QTextDocument *doc)
{
    QVector<QString> rst;
    int tot = doc->blockCount();        //回车符分隔block
    for (int i = 0; i < tot; i++) {
        QString line = doc->findBlockByNumber(i).text();
        line = line.simplified();
        if (line.isEmpty()) break;
        rst.push_back(line);
    }
    return rst;
}

QVector<double> MainWindow::vs2vd(const QVector<QString> &vec)
{
    QVector<double> rst;
    for (int i = 0; i < vec.size(); i++) {
        bool ok = true;
        QString s = vec[i];
        for (int idx = 0; ; ++idx) {
            double num = s.section(' ', idx, idx).toDouble(&ok);
            if (!ok) break;
            // two numbers on one line, error
            if (idx) return QVector<double>{};
            rst.push_back(num);
        }
    }
    return rst;
}

func_t MainWindow::getCustomFunc()
{
    static const QString header(
        "#include <vector>\n"
        "#include <cmath>\n"
        "using namespace std;\n"
        "typedef vector<double> ndarray;\n"
        "\n"
    );
    static const QString func_head(
        "__declspec (dllexport) ndarray func(double x, const ndarray &y) {\n"
        "    ndarray rst(y.size());\n"
    );
    static const QString func_tail(
        "    return rst;\n"
        "}\n"
    );

    auto func_code = doc2vec(ui->textEditInput_1->document());
    if (func_code.empty()) {
        QMessageBox::critical(this, "编译自定义函数", "请输入函数");
        return nullptr;
    }

    // generate dllfunc.cpp
    QFile file("dllfunc.cpp");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "编译自定义函数", "无法打开文件dllfunc.cpp");
        return nullptr;
    }
    QTextStream out(&file);
    out << header << func_head;
    for (int i = 0; i < func_code.size(); ++i)
        out << QString("    rst[%1] = ").arg(i) << func_code[i] << ";\n";
    out << func_tail;
//    out.flush();
    file.close();
    return compileFuncOnWindows();
}
