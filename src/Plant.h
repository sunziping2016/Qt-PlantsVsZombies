//
// Created by sun on 8/26/16.
//

#ifndef PLANTS_VS_ZOMBIES_PLANT_H
#define PLANTS_VS_ZOMBIES_PLANT_H

#include <QtCore>
#include <QtWidgets>

class MoviePixmapItem;

class GameScene;

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
    bool canEat, canSelect, canTrigger, night;
    double coolTime;
    int stature, sleep;
    int sunNum;
    QString toolTip;

    virtual double getDX() const;
    virtual double getDY(int x, int y) const;
    virtual bool canGrow(GameScene &scene, int x, int y) const;

protected:
    void updateSize();
};

class PlantInstance
{
public:
    PlantInstance(const Plant *plant);
    virtual ~PlantInstance() {}

    virtual void birth(GameScene &scene, double mx, double my, int c, int r);

private:
    const Plant *plantProtoType;

    int row, col;

    QGraphicsPixmapItem *shadowPNG;
    MoviePixmapItem *normalGif;
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

Plant *PlantFactory(const QString &eName);
PlantInstance *PlantInstanceFactory(const Plant *plant);

#endif //PLANTS_VS_ZOMBIES_PLANT_H
