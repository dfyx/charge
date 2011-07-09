#include <QtGui/QApplication>
#include "src/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Charge::MainWindow w;
    w.show();

    return a.exec();
}
