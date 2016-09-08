//
// Created by sun on 8/26/16.
//

#include "Zombie.h"
#include "GameScene.h"
#include "GameLevelData.h"
#include "ImageManager.h"
#include "MouseEventPixmapItem.h"
#include "Plant.h"
#include "Timer.h"

Zombie::Zombie()
    : hp(270), level(1), speed(1.5),
      aKind(0), attack(100),
      canSelect(true), canDisplay(true),
      beAttackedPointL(82), beAttackedPointR(156),
      breakPoint(90), sunNum(0), coolTime(0)
{}

bool Zombie::canPass(int row) const
{
    return scene->getGameLevelData()->LF[row] == 1;
}

void Zombie::update()
{
    QPixmap pic = gImageCache->load(staticGif);
    width = pic.width();
    height = pic.height();
}

Zombie1::Zombie1()
{
    eName = "oZombie";
    cName = tr("Zombie");
    QString path = "Zombies/Zombie/";
    cardGif = "Card/Zombies/Zombie.png";
    staticGif = path + "0.gif";
    normalGif = path + "Zombie.gif";
    attackGif = path + "ZombieAttack.gif";
    lostHeadGif = path + "ZombieLostHead.gif";
    lostHeadAttackGif = path + "ZombieLostHeadAttack.gif";
    headGif = path + "ZombieHead.gif";
    dieGif = path + "ZombieDie.gif";
    boomDieGif = path + "BoomDie.gif";
    standGif = path + "1.gif";

}

ZombieInstance::ZombieInstance(const Zombie *zombie)
    : zombieProtoType(zombie)
{
    uuid = QUuid::createUuid();
    hp = zombieProtoType->hp;
    speed = zombie->speed;
    altitude = 1;
    beAttacked = true;
    isAttacking = false;
    goingDie = false;
    picture = new MoviePixmapItem;
}

void ZombieInstance::birth(int row)
{
    ZX = attackedLX = zombieProtoType->scene->getCoordinate().getX(11);
    X = attackedLX - zombieProtoType->beAttackedPointL;
    attackedRX = X + zombieProtoType->beAttackedPointR;
    this->row = row;

    Coordinate &coordinate = zombieProtoType->scene->getCoordinate();
    picture->setMovie(zombieProtoType->normalGif);
    picture->setPos(X, coordinate.getY(row) - zombieProtoType->height - 10);
    picture->setZValue(3 * row + 1);
    shadowPNG = new QGraphicsPixmapItem(gImageCache->load("interface/plantShadow.png"));
    shadowPNG->setPos(zombieProtoType->beAttackedPointL - 10, zombieProtoType->height - 22);
    shadowPNG->setFlag(QGraphicsItem::ItemStacksBehindParent);
    shadowPNG->setParentItem(picture);
    picture->start();
    zombieProtoType->scene->addToGame(picture);
}

void ZombieInstance::checkActs()
{
    if (hp < 1) return;
    if (beAttacked && !isAttacking) {
        judgeAttack();
    }
    if (!isAttacking) {
        attackedRX -= speed;
        ZX = attackedLX -= speed;
        X -= speed;
        picture->setX(X);
        if (attackedRX < -50) {
            // TODO: Disappear
        }
        else if (attackedRX < 100) {
            // TODO: Lose
        }
    }
}

void ZombieInstance::judgeAttack()
{
    bool tempIsAttacking = false;
    PlantInstance *plant = nullptr;
    int col = zombieProtoType->scene->getCoordinate().getCol(ZX);
    if (col >= 1 && col <= 9) {
        auto plants = zombieProtoType->scene->getPlant(col, row);
        QList<int> keys = plants.keys();
        qSort(keys.begin(), keys.end(), [](int a, int b) { return b < a; });
        for (auto key: keys) {
            plant = plants[key];
            if (plant->plantProtoType->canEat && plant->attackedRX >= ZX && plant->attackedLX <= ZX) {
                tempIsAttacking = true;
                break;
            }
        }
    }
    if (tempIsAttacking != isAttacking) {
        isAttacking = tempIsAttacking;
        if (isAttacking) {
            picture->setMovie(zombieProtoType->attackGif);
        }
        else
            picture->setMovie(zombieProtoType->normalGif);
        picture->start();
    }
    if (isAttacking)
        normalAttack(plant);
}

void ZombieInstance::normalAttack(PlantInstance *plantInstance)
{
    QUuid plantUuid = plantInstance->uuid;
    (new Timer(this->picture, 1000, [this, plantUuid] {
        if (beAttacked) {
            PlantInstance *plant = zombieProtoType->scene->getPlant(plantUuid);
            if (plant)
                plant->getHurt(this, zombieProtoType->aKind, zombieProtoType->attack);
            judgeAttack();
        }
    }))->start();
}

ZombieInstance::~ZombieInstance()
{
    picture->deleteLater();
}

void ZombieInstance::crushDie()
{
    if (goingDie)
        return;
    goingDie =  true;
    hp = 0;
    MoviePixmapItem *goingDieHead = new MoviePixmapItem(zombieProtoType->headGif);
    goingDieHead->setPos(zombieProtoType->beAttackedPointL, -20);
    goingDieHead->setParentItem(picture);
    goingDieHead->start();
    shadowPNG->setPixmap(QPixmap());
    picture->stop();
    picture->setPixmap(QPixmap());
    (new Timer(picture, 2000, [this] {
        // TODO: Pole Vaulting Zombie
        zombieProtoType->scene->zombieDie(this);
    }))->start();
}

void ZombieInstance::getPea(int attack, int direction)
{
    getHit(attack);
}

void ZombieInstance::getHit(int attack)
{
    if (!beAttacked || goingDie)
        return;
    hp -= attack;
    if (hp < zombieProtoType->breakPoint) {
        //hp = 0;
        if (isAttacking)
            picture->setMovie(zombieProtoType->lostHeadAttackGif);
        else
            picture->setMovie(zombieProtoType->lostHeadGif);
        picture->start();
        MoviePixmapItem *goingDieHead = new MoviePixmapItem(zombieProtoType->headGif);
        goingDieHead->setPos(attackedLX, picture->y() - 20);
        goingDieHead->setZValue(picture->zValue());
        zombieProtoType->scene->addToGame(goingDieHead);
        goingDieHead->start();
        (new Timer(zombieProtoType->scene, 2000, [goingDieHead] {
            goingDieHead->deleteLater();
        }))->start();
        beAttacked = 0;
        autoReduceHp();
    }
    else {
        picture->setOpacity(0.5);
        (new Timer(picture, 100, [this] {
            picture->setOpacity(1);
        }))->start();
    }
}

void ZombieInstance::autoReduceHp()
{
    (new Timer(picture, 1000, [this] {
        hp-= 60;
        if (hp < 1)
            normalDie();
        else
            autoReduceHp();
    }))->start();
}

void ZombieInstance::normalDie()
{
    if (goingDie)
        return;
    goingDie =  true;
    hp = 0;
    picture->setMovie(zombieProtoType->dieGif);
    picture->start();
    (new Timer(picture, 2500, [this] {
        zombieProtoType->scene->zombieDie(this);
    }))->start();
}

Zombie *ZombieFactory(GameScene *scene, const QString &ename)
{
    Zombie *zombie = nullptr;
    if (ename == "oZombie")
        zombie = new Zombie1;
    if (zombie) {
        zombie->scene = scene;
        zombie->update();
    }
    return zombie;
}

ZombieInstance *ZombieInstanceFactory(const Zombie *zombie)
{
    return new ZombieInstance(zombie);
}
