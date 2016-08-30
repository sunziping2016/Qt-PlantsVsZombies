//
// Created by sun on 8/26/16.
//

#ifndef PLANTS_VS_ZOMBIES_GAMESCENE_H
#define PLANTS_VS_ZOMBIES_GAMESCENE_H


#include <QtWidgets>
#include "Coordinate.h"

class Plant;
class PlantInstance;
class Zombie;
class GameLevelData;
class MouseEventPixmapItem;
class MoviePixmapItem;
class PlantCardItem;
class TooltipItem;

class GameScene: public QGraphicsScene
{
    Q_OBJECT

public:
    GameScene(GameLevelData *gameLevel);
    ~GameScene();

    void setInfoText(QString text, const QColor &color = Qt::white);
    QPointF scenePos() const;
    GameLevelData *getGameLevelData() const;
    void addItemOnScreen(QGraphicsItem *item);
    void setItemOnScreenPos(QGraphicsItem *item, qreal x, qreal y);

    void beginCool();
    void doCoolTime(int index);
    void updateSunNum();
    void autoProduceSun(int sunNum);

    template <typename Functor>
    QTimeLine *moveItemTo(QGraphicsItem *item, qreal x, qreal y, int duration, Functor function)
    {
        QTimeLine *anim = new QTimeLine(duration, this);
        anim->setUpdateInterval(20);
        qreal fromX = item->x(), fromY = item->y();
        connect(anim, &QTimeLine::valueChanged, [item, fromX, fromY, x, y](qreal value) {
            item->setPos((x - fromX) * value + fromX, (y - fromY) * value + fromY);
        });
        connect(anim, &QTimeLine::finished, function);
        anim->start();
        return anim;
    }

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

    void initDisplayZombies();
    void initPlantCards();

signals:
    void mouseMove(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePress(QGraphicsSceneMouseEvent *mouseEvent);

public slots:
    void loadReady();
protected slots:
    void checkScroll();
    void letsGo();

private:
    QVector<QPair<QGraphicsItem *, qreal> > itemsOnScreen;

    GameLevelData *gameLevelData;
    QGraphicsPixmapItem *background;
    QGraphicsTextItem *techText;
    QGraphicsItemGroup *techTextGroup;
    QGraphicsItemGroup *displayZombiesGroup;
    QGraphicsItemGroup *menuGroup;
    QGraphicsTextItem *sunNumText;
    QGraphicsItemGroup *sunNumGroup;
    MouseEventPixmapItem *selectCardButtonReset, *selectCardButtonOkay;
    QGraphicsTextItem *selectCardTextReset, *selectCardTextOkay;
    QGraphicsItemGroup *selectCardGroup;
    QGraphicsItemGroup *selectedPlantGroup;
    QGraphicsPixmapItem *movePlantAlpha, *movePlant;
    MoviePixmapItem *imgGrowSoil, *imgGrowSpray;

    QMap<QString, Plant *> plantProtoTypes;
    QMap<QString, Zombie *> zombieProtoTypes;

    Coordinate coordinate;

    QVector<QPair<Zombie *, int> > zombieArray;
    QVector<Plant *> selectedPlantArray;
    QVector<QPair<PlantCardItem *, TooltipItem *> > selectedPlantGraphicsArray;
    QVector<QPair<bool, bool> > plantReady; // coolDown, sunNum
    QVector<PlantInstance *> plantInstances;
    QMap<int, QVector<QString> > mustShowAtFlag;

    int choose, sunNum;
};

#endif //PLANTS_VS_ZOMBIES_GAMESCENE_H
