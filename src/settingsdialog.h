#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "mainwindow.h"

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QPushButton>

namespace Charge
{
    class SettingsDialog : public QDialog
    {
        Q_OBJECT

        QLabel *resolutionLabel;
        QComboBox *resolutionBox;
        QGridLayout *layout;
        QPushButton *okButton;

        MainWindow *mainWindow;
    public:
        explicit SettingsDialog(QWidget *parent = 0);

    signals:

    public slots:
        void okPressed();
    };
}

#endif // SETTINGSDIALOG_H
