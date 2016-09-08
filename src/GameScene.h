//
// Created by sun on 8/26/16.
//

#ifndef PLANTS_VS_ZOMBIES_GAMESCENE_H
#define PLANTS_VS_ZOMBIES_GAMESCENE_H


#include <QtWidgets>
#include <QtMultimedia>
#include "Coordinate.h"
#include "Plant.h"
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

struct Trigger {
    Trigger(PlantInstance *plant, qreal from, qreal to, int direction, int id);

    PlantInstance *plant;
    qreal from, to;
    int direction;
    int id;
};

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
    void beginBGM();
    void beginCool();
    void beginSun(int sunNum);
    void beginZombies();
    void beginMonitor();

    void plantDie(PlantInstance *plant);
    void zombieDie(ZombieInstance *zombie);

    Plant *getPlantProtoType(const QString &eName);
    Zombie *getZombieProtoType(const QString &eName);

    QMap<int, PlantInstance *> getPlant(int col, int row);
    PlantInstance *getPlant(const QPointF &pos);
    PlantInstance *getPlant(const QUuid &uuid);
    ZombieInstance *getZombie(const QUuid &uuid);
    QList<ZombieInstance *> getZombieOnRow(int row);
    QList<ZombieInstance *> getZombieOnRowRange(int row, qreal from, qreal to);

    QPair<MoviePixmapItem *, std::function<void(bool)> > newSun(int sunNum);
    bool isCrater(int col, int row) const;
    bool isTombstone(int col, int row) const;
    Coordinate &getCoordinate();

    void addTrigger(int row, Trigger *trigger);

protected:
    void letsGo();
    void doCoolTime(int index);
    void updateTooltip(int index);
    void updateSunNum();
    void advanceFlag();
    void selectFlagZombie(int levelSum);

    static QPointF sizeToPoint(const QSizeF &size);

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
    QGraphicsPixmapItem *shovel;
    QGraphicsPixmapItem *shovelBackground;
    QGraphicsPixmapItem *movePlantAlpha, *movePlant;
    MoviePixmapItem *imgGrowSoil, *imgGrowSpray;
    FlagMeter *flagMeter;

    QMediaPlayer *backgroundMusic;
    QMediaPlayer *tapMusic;
    QMediaPlayer *sunMusic;
    QMediaPlayer *shovelMusic;
    QMediaPlayer *seedliftMusic;
    QMediaPlayer *plantMusic1, *plantMusic2;
    QMediaPlayer *groanMusic;
    QMediaPlayer *waveMusic;

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
    QMap<QPair<int, int>, QMap<int, PlantInstance *> > plantPosition;
    QList<QPair<int, int> > craters, tombstones;
    QList<QList<Trigger *> > plantTriggers;
    QList<QList<ZombieInstance *> > zombieRow;
    QMap<QUuid, PlantInstance *> plantUuid;
    QMap<QUuid, ZombieInstance *> zombieUuid;

    int choose, sunNum;
    QTimer *waveTimer;
    int waveNum;
};

#endif //PLANTS_VS_ZOMBIES_GAMESCENE_H
