//
// Created by sun on 8/25/16.
//

#ifndef PLANTS_VS_ZOMBIES_MAINVIEW_H
#define PLANTS_VS_ZOMBIES_MAINVIEW_H

#include <QtWidgets>
#include "Timer.h"

class SelectorScene;
class GameScene;
class MainWindow;

class MainView: public QGraphicsView
{
    Q_OBJECT

public:
    MainView(MainWindow *mainWindow);
    ~MainView();

    QString getUsername() const;
    void setUsername(const QString &username);
    MainWindow *getMainWindow() const;

    void switchToMenuScene();
    void switchToGameScene(const QString &eName);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    const int width, height;
    const QString usernameSettingEntry;

    SelectorScene *selectorScene;
    GameScene *gameScene;
    MainWindow *mainWindow;
};

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    QAction *getFullScreenAction() const;
private:
    const QString fullScreenSettingEntry;

    QGraphicsView *mainView;
    QAction *fullScreenAction;
};

extern MainView *gMainView;

#endif //PLANTS_VS_ZOMBIES_MAINVIEW_H
