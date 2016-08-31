//
// Created by sun on 8/26/16.
//

#include "Plant.h"
#include "ImageManager.h"
#include "GameScene.h"
#include "GameLevelData.h"
#include "MouseEventPixmapItem.h"

Plant::Plant() : hp(300),
                 pKind(1), bKind(0),
                 beAttackedPointL(20), beAttackedPointR(20),
                 canEat(true), canSelect(true), canTrigger(true), night(false),
                 coolTime(7.5), stature(0), sleep(0)
{

}

double Plant::getDX() const
{
    return -0.5 * width;
}

double Plant::getDY(int x, int y) const
{
    return -15;
}

bool Plant::canGrow(GameScene &scene, int x, int y) const
{
    return scene.getGameLevelData()->LF[y] == 1 && x >= 1 && x <=9;
}

void Plant::updateSize()
{
    QPixmap pic = gImageCache->load(staticGif);
    width = pic.width();
    height = pic.height();
}

Peashooter::Peashooter()
{
    eName = "oPeashooter";
    cName = "豌豆射手";
    beAttackedPointR = 51;
    sunNum = 100;
    cardGif = "Card/Plants/Peashooter.png";
    staticGif = "Plants/Peashooter/0.gif";
    normalGif = "Plants/Peashooter/Peashooter.gif";
    toolTip = "向敌人射出豌豆";
    updateSize();
}

SnowPea::SnowPea()
{
    eName = "oSnowPea";
    cName = "寒冰射手";
    bKind = -1;
    beAttackedPointR = 51;
    sunNum = 175;
    cardGif = "Card/Plants/SnowPea.png";
    staticGif = "Plants/SnowPea/0.gif";
    normalGif = "Plants/SnowPea/SnowPea.gif";
    toolTip = "寒冰射手可造成伤害, 同时又有减速效果";
    updateSize();
}

SunFlower::SunFlower()
{
    eName = "oSunFlower";
    cName = "向日葵";
    beAttackedPointR = 53;
    sunNum = 50;
    cardGif = "Card/Plants/SunFlower.png";
    staticGif = "Plants/SunFlower/0.gif";
    normalGif = "Plants/SunFlower/SunFlower1.gif";
    toolTip = "提供你额外的阳光";
    updateSize();
}

WallNut::WallNut()
{
    eName = "oWallNut";
    cName = "坚果墙";
    hp = 4000;
    beAttackedPointR = 45;
    sunNum = 50;
    coolTime = 30;
    cardGif = "Card/Plants/WallNut.png";
    staticGif = "Plants/WallNut/0.gif";
    normalGif = "Plants/WallNut/WallNut.gif";
    toolTip = "阻碍僵尸前进, 并保护你其他的植物";
    updateSize();
}

Plant *PlantFactory(const QString &eName)
{
    if (eName == "oPeashooter")
        return new Peashooter;
    if (eName == "oSnowPea")
        return new SnowPea;
    if (eName == "oSunFlower")
        return new SunFlower;
    if (eName == "oWallNut")
        return new WallNut;
    return nullptr;
}

PlantInstance::PlantInstance(const Plant *plant) : plantProtoType(plant)
{}

void PlantInstance::birth(GameScene &scene, double mx, double my, int c, int r)
{
    double x = mx + plantProtoType->getDX(), y = my + plantProtoType->getDY(c, r) - plantProtoType->height;
    graphicsGroup = new QGraphicsItemGroup;
    shadowPNG = new QGraphicsPixmapItem(gImageCache->load("interface/plantshadow32.png"));
    graphicsGroup->addToGroup(shadowPNG);
    shadowPNG->setPos(plantProtoType->width * 0.5 - 48, plantProtoType->height - 22);
    normalGif = new MoviePixmapItem(plantProtoType->normalGif);
    graphicsGroup->addToGroup(normalGif);
    graphicsGroup->setPos(x, y);
    scene.addItemOnScreen(graphicsGroup);
    normalGif->start();
}

PlantInstance *PlantInstanceFactory(const Plant *plant)
{
    return new PlantInstance(plant);
}