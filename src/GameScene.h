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

    void setInfoText(const QString &text);
    GameLevelData *getGameLevelData() const;

    void beginCool();
    void doCoolTime(int index);
    void updateTooltip(int index);
    void updateSunNum();
    void autoProduceSun(int sunNum);
    void customSpecial(const QString &name, int col, int row);
    void addToGame(QGraphicsItem *item);

    void loadReady();
    void loadAcessFinished();

protected:
    void letsGo();

    static QPointF SizeToPoint(const QSizeF &size);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

signals:
    void mouseMove(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePress(QGraphicsSceneMouseEvent *mouseEvent);

private:
    QVector<QPair<QGraphicsItem *, qreal> > itemsOnScreen;

    GameLevelData *gameLevelData;

    QGraphicsPixmapItem *background;
    QGraphicsItemGroup *gameGroup;
    QGraphicsSimpleTextItem *infoText;
    QGraphicsRectItem *infoTextGroup;
    MouseEventPixmapItem *menuGroup;
    QGraphicsSimpleTextItem *sunNumText;
    QGraphicsPixmapItem *sunNumGroup;
    MouseEventPixmapItem *selectCardButtonReset, *selectCardButtonOkay;
    QGraphicsSimpleTextItem *selectCardTextReset, *selectCardTextOkay;
    QGraphicsPixmapItem *selectingPanel;
    QGraphicsItemGroup *cardPanel;
    QGraphicsPixmapItem *movePlantAlpha, *movePlant;
    MoviePixmapItem *imgGrowSoil, *imgGrowSpray;

    QMap<QString, Plant *> plantProtoTypes;
    QMap<QString, Zombie *> zombieProtoTypes;

    Coordinate coordinate;

    QList<QPair<Zombie *, int> > zombieArray;
    QList<Plant *> selectedPlantArray;
    struct CardGraphicsItem {
        PlantCardItem *plantCard;
        TooltipItem *tooltip;
    };
    QList<CardGraphicsItem > cardGraphics;
    struct CardReadyItem {
        bool cool;
        bool sun;
    };
    QList<CardReadyItem> cardReady;
    QList<PlantInstance *> plantInstances;
    QMap<int, QVector<QString> > mustShowAtFlag;

    int choose, sunNum;
};

#endif //PLANTS_VS_ZOMBIES_GAMESCENE_H
