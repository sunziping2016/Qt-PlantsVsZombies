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
                 zIndex(0),
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

PlantInstance::PlantInstance(const Plant *plant) : plantProtoType(plant)
{}

void PlantInstance::birth(GameScene &scene, double mx, double my, int c, int r)
{
    double x = mx + plantProtoType->getDX(), y = my + plantProtoType->getDY(c, r) - plantProtoType->height;
    col = c, row = r;
    normalGif = new MoviePixmapItem(plantProtoType->normalGif);
    normalGif->setPos(x, y);
    normalGif->setZValue(plantProtoType->zIndex + 3 * my);
    shadowPNG = new QGraphicsPixmapItem(gImageCache->load("interface/plantShadow.png"));
    shadowPNG->setPos(plantProtoType->width * 0.5 - 48, plantProtoType->height - 22);
    shadowPNG->setFlag(QGraphicsItem::ItemStacksBehindParent);
    shadowPNG->setParentItem(normalGif);
    normalGif->start();
    scene.addToGame(normalGif);
}

Peashooter::Peashooter()
{
    eName = "oPeashooter";
    cName = tr("Peashooter");
    beAttackedPointR = 51;
    sunNum = 100;
    cardGif = "Card/Plants/Peashooter.png";
    staticGif = "Plants/Peashooter/0.gif";
    normalGif = "Plants/Peashooter/Peashooter.gif";
    toolTip = tr("Shoots peas at zombies");
    updateSize();
}

SnowPea::SnowPea()
{
    eName = "oSnowPea";
    cName = tr("Snow Pea");
    bKind = -1;
    beAttackedPointR = 51;
    sunNum = 175;
    cardGif = "Card/Plants/SnowPea.png";
    staticGif = "Plants/SnowPea/0.gif";
    normalGif = "Plants/SnowPea/SnowPea.gif";
    toolTip = tr("Slows down zombies with cold precision");
    updateSize();
}

SunFlower::SunFlower()
{
    eName = "oSunflower";
    cName = tr("Sunflower");
    beAttackedPointR = 53;
    sunNum = 50;
    cardGif = "Card/Plants/SunFlower.png";
    staticGif = "Plants/SunFlower/0.gif";
    normalGif = "Plants/SunFlower/SunFlower1.gif";
    toolTip = tr("Makes extra Sun for placing plants");
    updateSize();
}

WallNut::WallNut()
{
    eName = "oWallNut";
    cName = tr("Wall-nut");
    hp = 4000;
    beAttackedPointR = 45;
    sunNum = 50;
    coolTime = 30;
    cardGif = "Card/Plants/WallNut.png";
    staticGif = "Plants/WallNut/0.gif";
    normalGif = "Plants/WallNut/WallNut.gif";
    toolTip = tr("Stops zombies with its chewy shell");
    updateSize();
}

LawnCleaner::LawnCleaner()
{
    eName = "oLawnCleaner";
    cName = tr("Lawn Cleaner");
    beAttackedPointL = 0;
    beAttackedPointR = 71;
    sunNum = 0;
    staticGif = normalGif = "interface/LawnCleaner.png";
    canEat = 0;
    stature = 1;
    toolTip = tr("Normal lawn cleaner");
    updateSize();
}


PoolCleaner::PoolCleaner()
{
    eName = "oPoolCleaner";
    cName = tr("Pool Cleaner");
    beAttackedPointR = 47;
    staticGif = normalGif = "interface/PoolCleaner.png";
    toolTip = tr("Pool Cleaner");
    updateSize();
}

Plant *PlantFactory(const QString &eName)
{
    if (eName == "oPeashooter")
        return new Peashooter;
    if (eName == "oSnowPea")
        return new SnowPea;
    if (eName == "oSunflower")
        return new SunFlower;
    if (eName == "oWallNut")
        return new WallNut;
    if (eName == "oLawnCleaner")
        return new LawnCleaner;
    if (eName == "oPoolCleaner")
        return new PoolCleaner;
    return nullptr;
}

PlantInstance *PlantInstanceFactory(const Plant *plant)
{
    return new PlantInstance(plant);
}
