//
// Created by sun on 8/25/16.
//

#ifndef PLANTS_VS_ZOMBIES_MAINWINDOW_H
#define PLANTS_VS_ZOMBIES_MAINWINDOW_H

#include <QtWidgets>

class MenuScene;
class GameScene;

class MainWindow: public QGraphicsView
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    const QString & getUsername() const;
    void setUsername(const QString &username);

    template <typename Functor>
    void scrollToX(int toX, int duration, Functor functor)
    {
        int fromX = horizontalScrollBar()->value();
        QTimeLine *anim = new QTimeLine(duration, this);
        anim->setUpdateInterval(20);
        connect(anim, &QTimeLine::valueChanged, [this, fromX, toX](qreal x){
            setX(fromX + x * (toX - fromX));
        });
        connect(anim, &QTimeLine::finished, functor);
        anim->start();
    }

protected:
    virtual void wheelEvent (QWheelEvent *event) {}
    virtual void keyPressEvent(QKeyEvent *event) {}

public slots:

    void switchToMenuScene();
    void switchToGameScene(const QString &eName);

    void setX(qreal x);

private:
    MenuScene *menuScene;
    GameScene *gameScene;

    QString username;
};

extern MainWindow *gMainWindow;

#endif //PLANTS_VS_ZOMBIES_MAINWINDOW_H
