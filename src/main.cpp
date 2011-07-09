#include <QtGui/QApplication>
#include "settingsdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Charge::SettingsDialog d;
    d.show();

    return a.exec();
}
