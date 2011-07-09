#include "settingsdialog.h"
#include "mainwindow.h"

namespace Charge
{
    SettingsDialog::SettingsDialog(QWidget *parent) :
        QDialog(parent)
    {
        layout = new QGridLayout(this);

        resolutionLabel = new QLabel("Resolution");
        layout->addWidget(resolutionLabel, 0, 0, 1, 1);

        resolutionBox = new QComboBox();
        resolutionBox->addItem("640x480", QSize(640, 480));
        resolutionBox->addItem("800x600", QSize(800, 600));
        resolutionBox->addItem("1024x768", QSize(1024, 768));
        resolutionBox->addItem("Fullscreen");
        layout->addWidget(resolutionBox, 0, 1, 1, 1);

        okButton = new QPushButton("&OK", this);
        connect(okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
        layout->addWidget(okButton, 2, 1, 1, 1);

        setLayout(layout);
    }

    void SettingsDialog::okPressed()
    {
        mainWindow = new MainWindow();

        if(resolutionBox->currentText() == "Fullscreen")
        {
            mainWindow->showFullScreen();
        }
        else
        {
            mainWindow->resize(resolutionBox->itemData(resolutionBox->currentIndex()).toSize());
            mainWindow->show();
        }
        hide();
    }
}
