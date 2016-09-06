//
// Created by sun on 8/26/16.
//

#ifndef PLANTS_VS_ZOMBIES_PLANT_H
#define PLANTS_VS_ZOMBIES_PLANT_H

#include <QtCore>
#include <QtWidgets>

class MoviePixmapItem;
class GameScene;
class ZombieInstance;
class Trigger;

class Plant
{
    Q_DECLARE_TR_FUNCTIONS(Plant)

public:
    Plant();
    virtual  ~Plant() {}

    QString eName, cName;
    int width, height;
    int hp, pKind, bKind;
    int beAttackedPointL, beAttackedPointR;
    int zIndex;
    QString cardGif, staticGif, normalGif;
    bool canEat, canSelect, night;
    double coolTime;
    int stature, sleep;
    int sunNum;
    QString toolTip;

    virtual double getDX() const;
    virtual double getDY(int x, int y) const;
    virtual bool canGrow(int x, int y) const;

    GameScene *scene;
    void update();
};

class PlantInstance
{
public:
    PlantInstance(const Plant *plant);
    virtual ~PlantInstance();

    virtual void birth(int c, int r);
    virtual void initTrigger();
    virtual void triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger);
    virtual void normalAttack(ZombieInstance *zombieInstance);

    bool contains(const QPointF &pos);

    const Plant *plantProtoType;

    QUuid uuid;
    int row, col;
    bool canTrigger;
    qreal attackedLX, attackedRX;
    QList<Trigger *> triggers;

    QGraphicsPixmapItem *shadowPNG;
    MoviePixmapItem *picture;
};

class Peashooter: public Plant
{
    Q_DECLARE_TR_FUNCTIONS(Peashooter)
public:
    Peashooter();
};

class SnowPea: public Peashooter
{
    Q_DECLARE_TR_FUNCTIONS(SnowPea)
public:
    SnowPea();
};

class SunFlower: public Plant
{
    Q_DECLARE_TR_FUNCTIONS(SunFlower)
public:
    SunFlower();
};

class SunFlowerInstance: public PlantInstance
{
public:
    SunFlowerInstance(const Plant *plant);
    virtual void initTrigger();
private:
    QString lightedGif;
};

class WallNut: public Plant
{
    Q_DECLARE_TR_FUNCTIONS(WallNut)
public:
    WallNut();
};

class LawnCleaner: public Plant
{
    Q_DECLARE_TR_FUNCTIONS(LawnCleaner)
public:
    LawnCleaner();
};

class PoolCleaner: public LawnCleaner
{
    Q_DECLARE_TR_FUNCTIONS(PoolCleaner)
public:
    PoolCleaner();
};

Plant *PlantFactory(GameScene *scene, const QString &eName);
PlantInstance *PlantInstanceFactory(const Plant *plant);

#endif //PLANTS_VS_ZOMBIES_PLANT_H
