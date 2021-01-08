#include "mainwindow.h"

#include <QApplication>

//#include <iostream>
//using namespace std;
//#define debug(x) cout << __FUNCTION__ << (": " #x "\n") << (x) << endl;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
