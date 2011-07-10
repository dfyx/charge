#include "mainwindow.h"
#include "chargeloader.h"

namespace Charge
{
    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent)
    {
        ChargeLoader loader;
        field = (CircularField*) loader.loadField("test.charge");

        canvas = new MainCanvas(field, this);
        setCentralWidget(canvas);
    }

    MainWindow::~MainWindow()
    {
        delete field;
    }

    void MainWindow::keyPressEvent(QKeyEvent *event)
    {
        if(event->key() == Qt::Key_A)
        {
            field->reactToPlayer(1);
        }
        else if(event->key() == Qt::Key_L)
        {
            field->reactToPlayer(2);
        }
    }
}
