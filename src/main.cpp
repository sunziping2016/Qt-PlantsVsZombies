//
// Created by sun on 8/25/16.
//

#include <QtCore>
#include <QtWidgets>
#include "MainWindow.h"
#include "ImageManager.h"

int main(int argc, char * *argv)
{
    QApplication app(argc, argv);
    InitImageManager();
    qsrand((uint) QTime::currentTime().msec());
    MainWindow mainWindow;
    mainWindow.switchToMenuScene();
    mainWindow.show();
    int res = app.exec();
    DestoryImageManager();
    return res;
}


