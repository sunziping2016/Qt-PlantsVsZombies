//
// Created by sun on 8/26/16.
//

#ifndef PLANTS_VS_ZOMBIES_GAMESCENE_H
#define PLANTS_VS_ZOMBIES_GAMESCENE_H


#include <QtWidgets>
#include "Coordinate.h"
#include "GameLevelData.h"
#include "Zombie.h"

class Plant;
class PlantInstance;
class Zombie;
class GameLevelData;
class MouseEventPixmapItem;
class MoviePixmapItem;
class PlantCardItem;
class TooltipItem;
class Zombie;
class ZombieInstance;

class FlagMeter: public QGraphicsPixmapItem
{
public:
    FlagMeter(GameLevelData *gameLevelData);

    void updateFlagZombies(int flagZombies);
private:
    int flagNum;
    qreal flagHeadStep;
    QPixmap flagMeterEmpty, flagMeterFull;
    QGraphicsPixmapItem *flagTitle, *flagHead;
    QMap<int, QGraphicsPixmapItem *> flags;
};

class GameScene: public QGraphicsScene
{
    Q_OBJECT

public:
    GameScene(GameLevelData *gameLevel);
    ~GameScene();

    void setInfoText(const QString &text);
    GameLevelData *getGameLevelData() const;

    void addToGame(QGraphicsItem *item);
    void customSpecial(const QString &name, int col, int row);
    void prepareGrowPlants(std::function<void(void)> functor);

    void loadReady();
    void loadAcessFinished();
    void beginCool();
    void beginSun(int sunNum);
    void beginZombies();

    void plantDie(PlantInstance *plant);
    void zombieDie(ZombieInstance *zombie);
protected:
    void letsGo();
    void doCoolTime(int index);
    void updateTooltip(int index);
    void updateSunNum();
    void advanceFlag();
    void selectFlagZombie(int levelSum);

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
    FlagMeter *flagMeter;

    QMap<QString, Plant *> plantProtoTypes;
    QMap<QString, Zombie *> zombieProtoTypes;

    Coordinate coordinate;

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
    QList<ZombieInstance *> zombieInstances;

    int choose, sunNum;
    QTimer *waveTimer;
    int waveNum;
};

#endif //PLANTS_VS_ZOMBIES_GAMESCENE_H
