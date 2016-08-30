//
// Created by sun on 8/25/16.
//

#include "MainWindow.h"
#include "GameLevelData.h"
#include "MenuScene.h"
#include "GameScene.h"

MainWindow::MainWindow()
        : gameScene(nullptr)
{
    gMainWindow = this;

    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::TextAntialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);

    setWindowTitle("植物大战僵尸");
    setFixedSize(900, 600);
    setFrameStyle(0);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setBackgroundBrush(Qt::black);

    username = QDir::home().dirName();

    menuScene = new MenuScene;
}

MainWindow::~MainWindow()
{
    delete menuScene;
    if (gameScene)
        delete gameScene;
}

void MainWindow::switchToGameScene(const QString &eName)
{
    GameScene *newGameScene = new GameScene(GameLevelDataFactory(eName));
    setScene(newGameScene);
    if (gameScene)
        delete gameScene;
    gameScene = newGameScene;
    gameScene->loadReady();
}


MainWindow *gMainWindow;

const QString &MainWindow::getUsername() const
{
    return username;
}

void MainWindow::setUsername(const QString &username)
{
    MainWindow::username = username;
}

void MainWindow::switchToMenuScene()
{
    setScene(menuScene);
}

void MainWindow::setX(qreal x)
{
    horizontalScrollBar()->setValue((int) x);
}
