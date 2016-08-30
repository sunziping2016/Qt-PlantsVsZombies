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
public:
    Plant();
    virtual  ~Plant() {}

    QString eName, cName;
    int width, height;
    int hp, pKind, bKind;
    int beAttackedPointL, beAttackedPointR;
    QString cardGif, staticGif, normalGif;
    bool canEat, canSelect, canTrigger, night;
    double coolTime;
    int stature, sleep;
    int sunNum;
    QString toolTip;

    virtual double getDX() const;
    // TODO:
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

    QGraphicsItemGroup *graphicsGroup;
    QGraphicsPixmapItem *shadowPNG;
    MoviePixmapItem *normalGif;
};

class Peashooter: public Plant
{
public:
    Peashooter();
};

class SnowPea: public Peashooter
{
public:
    SnowPea();
};

class SunFlower: public Plant
{
public:
    SunFlower();
};

class WallNut: public Plant
{
public:
    WallNut();
};

Plant * PlantFactory(const QString &eName);
PlantInstance * PlantInstanceFactory(const Plant *plant);

#endif //PLANTS_VS_ZOMBIES_PLANT_H
