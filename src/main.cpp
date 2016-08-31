//
// Created by sun on 8/25/16.
//

#include <QtCore>
#include <QtWidgets>
#include "MainView.h"
#include "ImageManager.h"

int main(int argc, char * *argv)
{
    // For QSettings
    QCoreApplication::setOrganizationName("Sun Ziping");
    QCoreApplication::setOrganizationDomain("sunziping.com");
    QCoreApplication::setApplicationName("Plants vs Zombies");

    QApplication app(argc, argv);
    InitImageManager();
    qsrand((uint) QTime::currentTime().msec());
    MainWindow mainWindow;
    gMainView->switchToMenuScene();
    mainWindow.show();
    int res = app.exec();
    DestoryImageManager();
    return res;
}


