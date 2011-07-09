#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "maincanvas.h"

namespace Charge
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

        CircularField *field;
        MainCanvas *canvas;
    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        virtual void keyPressEvent(QKeyEvent *event);

    signals:

    public slots:

    };
}

#endif // MAINWINDOW_H
