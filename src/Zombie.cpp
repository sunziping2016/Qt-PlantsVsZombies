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
    cardGif = "Card/Zombies/Zombie.png";
    QString path = "Zombies/Zombie/";
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

Zombie2::Zombie2()
{
    eName = "oZombie2";
    normalGif = "Zombies/Zombie/Zombie2.gif";
    standGif = "Zombies/Zombie/2.gif";
}

Zombie3::Zombie3()
{
    eName = "oZombie3";
    normalGif = "Zombies/Zombie/Zombie3.gif";
    standGif = "Zombies/Zombie/3.gif";
}

FlagZombie::FlagZombie()
{
    eName = "oFlagZombie";
    cName = tr("Flag Zombie");
    speed = 2.2;
    beAttackedPointR = 101;
    QString path = "Zombies/FlagZombie/";
    cardGif = "Card/Zombies/FlagZombie.png";
    staticGif = path + "0.gif";
    normalGif = path + "FlagZombie.gif";QSound::play(":/audio/splat1.wav");
    attackGif = path + "FlagZombieAttack.gif";
    lostHeadGif = path + "FlagZombieLostHead.gif";
    lostHeadAttackGif = path + "FlagZombieLostHeadAttack.gif";
    standGif = path + "1.gif";
}

ZombieInstance::ZombieInstance(const Zombie *zombie)
    : zombieProtoType(zombie), frozenTimer(nullptr), picture(new MoviePixmapItem)
{
    uuid = QUuid::createUuid();
    hp = zombieProtoType->hp;
    orignSpeed = speed = zombie->speed;
    orignAttack = attack = zombie->attack;
    altitude = 1;
    beAttacked = true;
    isAttacking = false;
    goingDie = false;
    normalGif = zombie->normalGif;
    attackGif = zombie->attackGif;
    lostHeadGif = zombie->lostHeadGif;
    lostHeadAttackGif = zombie->lostHeadAttackGif;
}

void ZombieInstance::birth(int row)
{
    ZX = attackedLX = zombieProtoType->scene->getCoordinate().getX(11);
    X = attackedLX - zombieProtoType->beAttackedPointL;
    attackedRX = X + zombieProtoType->beAttackedPointR;
    this->row = row;

    Coordinate &coordinate = zombieProtoType->scene->getCoordinate();
    picture->setMovie(normalGif);
    picture->setPos(X, coordinate.getY(row) - zombieProtoType->height - 10);
    picture->setZValue(3 * row + 1);
    shadowPNG = new QGraphicsPixmapItem(gImageCache->load("interface/plantShadow.png"));
    shadowPNG->setPos(getShadowPos());
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
            zombieProtoType->scene->zombieDie(this);
        }
        else if (attackedRX < 100) {
            zombieProtoType->scene->gameLose();
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
            picture->setMovie(attackGif);
        }
        else
            picture->setMovie(normalGif);
        picture->start();
    }
    if (isAttacking)
        normalAttack(plant);
}

void ZombieInstance::normalAttack(PlantInstance *plantInstance)
{
    if (qrand() % 2)
        QSound::play(":/audio/chomp.wav");
    else
        QSound::play(":/audio/chompsoft.wav");
    (new Timer(this->picture, 500, [this] {
        if (qrand() % 2)
            QSound::play(":/audio/chomp.wav");
        else
            QSound::play(":/audio/chompsoft.wav");
    }))->start();
    QUuid plantUuid = plantInstance->uuid;
    (new Timer(this->picture, 1000, [this, plantUuid] {
        if (beAttacked) {
            PlantInstance *plant = zombieProtoType->scene->getPlant(plantUuid);
            if (plant)
                plant->getHurt(this, zombieProtoType->aKind, attack);
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
    beAttacked = false;
    hp = 0;
    MoviePixmapItem *goingDieHead = new MoviePixmapItem(zombieProtoType->headGif);
    goingDieHead->setPos(getDieingHeadPos());
    zombieProtoType->scene->addToGame(goingDieHead);
    goingDieHead->start();
    shadowPNG->setPixmap(QPixmap());
    picture->stop();
    picture->setPixmap(QPixmap());
    (new Timer(picture, 2000, [this, goingDieHead] {
        goingDieHead->deleteLater();
        zombieProtoType->scene->zombieDie(this);
    }))->start();
}

void ZombieInstance::getPea(int attack, int direction)
{
    playNormalballAudio();
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
            picture->setMovie(lostHeadAttackGif);
        else
            picture->setMovie(lostHeadGif);
        picture->start();
        MoviePixmapItem *goingDieHead = new MoviePixmapItem(zombieProtoType->headGif);
        goingDieHead->setPos(getDieingHeadPos());
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

void ZombieInstance::playNormalballAudio()
{
    switch (qrand() % 3) {
        case 0: QSound::play(":/audio/splat1.wav"); break;
        case 1: QSound::play(":/audio/splat2.wav"); break;
        default: QSound::play(":/audio/splat3.wav"); break;
    }
}

QPointF ZombieInstance::getShadowPos()
{
    return QPointF(zombieProtoType->beAttackedPointL - 10, zombieProtoType->height - 22);
}

QPointF ZombieInstance::getDieingHeadPos()
{
    return QPointF(attackedLX, picture->y() - 20);
}

bool ZombieInstance::getCrushed(PlantInstance *instance)
{
    return true;
}

void ZombieInstance::getSnowPea(int attack, int direction)
{
    if (frozenTimer) {
        frozenTimer->stop();
        frozenTimer->deleteLater();
    }
    speed = orignSpeed / 2;
    this->attack = 50;
    (frozenTimer = new Timer(picture, 10000, [this] {
        frozenTimer = nullptr;
        speed = orignSpeed;
        this->attack = orignAttack;
    }))->start();
    playSlowballAudio();
    getHit(attack);
}

void ZombieInstance::playSlowballAudio()
{
    QSound::play(":/audio/frozen.wav");
}

void ZombieInstance::getFirePea(int attack, int direction)
{
    if (frozenTimer) {
        frozenTimer->stop();
        frozenTimer->deleteLater();
        frozenTimer = nullptr;
        speed = orignSpeed;
        this->attack = orignAttack;
    }
    playFireballAudio();
    getHit(attack);
}

void ZombieInstance::playFireballAudio()
{
    if (qrand() % 2)
        QSound::play(":/audio/ignite.wav");
    else
        QSound::play(":/audio/ignite2.wav");
}


OrnZombieInstance1::OrnZombieInstance1(const Zombie *zombie)
    : ZombieInstance(zombie)
{
    ornHp = getZombieProtoType()->ornHp;
    hasOrnaments = true;
}

const OrnZombie1 *OrnZombieInstance1::getZombieProtoType()
{
    return static_cast<const OrnZombie1 *>(zombieProtoType);
}

void OrnZombieInstance1::getHit(int attack)
{
    if (hasOrnaments) {
        ornHp -= attack;
        if (ornHp < 1) {
            hp += ornHp;
            hasOrnaments = false;
            normalGif = getZombieProtoType()->ornLostNormalGif;
            attackGif = getZombieProtoType()->ornLostAttackGif;
            picture->setMovie(isAttacking ? attackGif : normalGif);
            picture->start();
        }
        picture->setOpacity(0.5);
        (new Timer(picture, 100, [this] {
            picture->setOpacity(1);
        }))->start();
    }
    else
        ZombieInstance::getHit(attack);
}

ConeheadZombie::ConeheadZombie()
{
    eName = "oConeheadZombie";
    cName = tr("Conehead Zombie");
    ornHp = 370;
    level = 2;
    sunNum = 75;
    QString path = "Zombies/ConeheadZombie/";
    cardGif = "Card/Zombies/ConeheadZombie.png";
    staticGif = path + "0.gif";
    normalGif = path + "ConeheadZombie.gif";
    attackGif = path + "ConeheadZombieAttack.gif";
    ornLostNormalGif =  "Zombies/Zombie/Zombie.gif";
    ornLostAttackGif = "Zombies/Zombie/ZombieAttack.gif";
    standGif = path + "1.gif";
}

ConeheadZombieInstance::ConeheadZombieInstance(const Zombie *zombie)
    : OrnZombieInstance1(zombie)
{}

void ConeheadZombieInstance::playNormalballAudio()
{
    if (hasOrnaments)
        QSound::play(":/audio/plastichit.wav");
    else
        OrnZombieInstance1::playNormalballAudio();
}

BucketheadZombieInstance::BucketheadZombieInstance(const Zombie *zombie)
    : OrnZombieInstance1(zombie)
{}

void BucketheadZombieInstance::playNormalballAudio()
{
    if (hasOrnaments) {
        if (qrand() % 2)
            QSound::play(":/audio/shieldhit.wav");
        else
            QSound::play(":/audio/shieldhit2.wav");
    }
    else
        OrnZombieInstance1::playNormalballAudio();
}

BucketheadZombie::BucketheadZombie()
{
    eName = "oBucketheadZombie";
    cName = tr("Buckethead Zombie");
    ornHp = 1100;
    level = 3;
    sunNum = 125;
    QString path = "Zombies/BucketheadZombie/";
    cardGif = "Card/Zombies/BucketheadZombie.png";
    staticGif = path + "0.gif";
    normalGif = path + "BucketheadZombie.gif";
    attackGif = path + "BucketheadZombieAttack.gif";
    ornLostNormalGif =  "Zombies/Zombie/Zombie2.gif";
    standGif = path + "1.gif";
}

PoleVaultingZombie::PoleVaultingZombie()
{
    eName = "oPoleVaultingZombie";
    cName = tr("Pole Vaulting Zombie");
    hp = 500;
    speed = 3.2;
    beAttackedPointL = 215;
    beAttackedPointR = 260;
    level = 2;
    sunNum = 75;
    QString path = "Zombies/PoleVaultingZombie/";
    cardGif = "Card/Zombies/PoleVaultingZombie.png";
    staticGif = path + "0.gif";
    normalGif = path + "PoleVaultingZombie.gif";
    attackGif = path + "PoleVaultingZombieAttack.gif";
    lostHeadGif = path + "PoleVaultingZombieLostHead.gif";
    lostHeadAttackGif = path + "PoleVaultingZombieLostHeadAttack.gif";
    headGif = path + "PoleVaultingZombieHead.gif";
    dieGif = path + "PoleVaultingZombieDie.gif";
    boomDieGif = path + "BoomDie.gif";
    walkGif = path + "PoleVaultingZombieWalk.gif";
    lostHeadWalkGif = path + "PoleVaultingZombieLostHeadWalk.gif";
    jumpGif1 = path + "PoleVaultingZombieJump.gif";
    jumpGif2 = path + "PoleVaultingZombieJump2.gif";
    standGif = path + "1.gif";
}

PoleVaultingZombieInstance::PoleVaultingZombieInstance(const Zombie *zombie)
    : ZombieInstance(zombie)
{
    judgeAttackOrig = false;
    lostPole = false;
    beginCrushed = false;
}

const PoleVaultingZombie *PoleVaultingZombieInstance::getZombieProtoType()
{
    return static_cast<const PoleVaultingZombie *>(zombieProtoType);
}

QPointF PoleVaultingZombieInstance::getShadowPos()
{
    return QPointF(zombieProtoType->beAttackedPointL - 20, zombieProtoType->height - 35);
}

QPointF PoleVaultingZombieInstance::getDieingHeadPos()
{
    return QPointF(X, picture->y() - 20);
}

void PoleVaultingZombieInstance::judgeAttack()
{
    if (judgeAttackOrig)
        ZombieInstance::judgeAttack();
    else {
        int colEnd = zombieProtoType->scene->getCoordinate().getCol(ZX);
        for (int col = colEnd - 2; col <= colEnd; ++col) {
            if (col > 9) continue;
            QMap<int, PlantInstance *> plants = zombieProtoType->scene->getPlant(col, row);
            for (int i = 2; i >= 0; --i) {
                if (!plants.contains(i)) continue;
                PlantInstance *plant = plants[i];
                if (plant->attackedRX >= ZX - 74 && plant->attackedLX < ZX && plant->plantProtoType->canEat) {
                    judgeAttackOrig = true;
                    posX = plant->attackedLX;
                    normalAttack(plant);
                    break;
                }
            }
        }
    }
}

void PoleVaultingZombieInstance::normalAttack(PlantInstance *plantInstance)
{
    if (lostPole)
        ZombieInstance::normalAttack(plantInstance);
    else {
        QSound::play(":/audio/grassstep.wav");
        picture->setMovie(getZombieProtoType()->jumpGif1);
        picture->start();
        shadowPNG->setVisible(false);
        isAttacking = true;
        altitude = 2;
        (new Timer(picture, 500, [] { QSound::play(":/audio/polevault.wav"); }))->start();
        QUuid plantUuid = plantInstance->uuid;
        (new Timer(picture, 1000, [this, plantUuid] {
            PlantInstance *plant = zombieProtoType->scene->getPlant(plantUuid);
            if (plant && plant->plantProtoType->stature > 0) {
                attackedLX = ZX = plant->attackedRX;
                X = attackedLX - zombieProtoType->beAttackedPointL;
                attackedRX = X + zombieProtoType->beAttackedPointR;
                picture->setX(X);
                picture->setMovie(getZombieProtoType()->walkGif);
                picture->start();
                shadowPNG->setVisible(true);
                isAttacking = 0;
                altitude = 1;
                orignSpeed = speed = 1.6;
                normalGif = getZombieProtoType()->walkGif;
                lostHeadGif = getZombieProtoType()->lostHeadWalkGif;
                lostPole = true;
                judgeAttackOrig = true;
            }
            else {
                attackedRX = posX;
                X = attackedRX - zombieProtoType->beAttackedPointR;
                attackedLX = ZX = X + zombieProtoType->beAttackedPointL;
                picture->setX(X);
                picture->setMovie(getZombieProtoType()->jumpGif2);
                picture->start();
                shadowPNG->setVisible(true);
                (new Timer(picture, 800, [this]{
                    picture->setMovie(getZombieProtoType()->walkGif);
                    picture->start();
                    isAttacking = 0;
                    altitude = 1;
                    orignSpeed = speed = 1.6;
                    normalGif = getZombieProtoType()->walkGif;
                    lostHeadGif = getZombieProtoType()->lostHeadWalkGif;
                    lostPole = true;
                    judgeAttackOrig = true;
                }))->start();
            }
        }))->start();
    }
}

Zombie *ZombieFactory(GameScene *scene, const QString &ename)
{
    Zombie *zombie = nullptr;
    if (ename == "oZombie")
        zombie = new Zombie1;
    if (ename == "oZombie2")
        zombie = new Zombie2;
    if (ename == "oZombie3")
        zombie = new Zombie3;
    if (ename == "oFlagZombie")
        zombie = new FlagZombie;
    if (ename == "oConeheadZombie")
        zombie = new ConeheadZombie;
    if (ename == "oBucketheadZombie")
        zombie = new BucketheadZombie;
    if (ename == "oPoleVaultingZombie")
        zombie = new PoleVaultingZombie;
    if (zombie) {
        zombie->scene = scene;
        zombie->update();
    }
    return zombie;
}

ZombieInstance *ZombieInstanceFactory(const Zombie *zombie)
{
    if (zombie->eName == "oConeheadZombie")
        return new ConeheadZombieInstance(zombie);
    if (zombie->eName == "oBucketheadZombie")
        return new BucketheadZombieInstance(zombie);
    if (zombie->eName == "oPoleVaultingZombie")
        return new PoleVaultingZombieInstance(zombie);
    return new ZombieInstance(zombie);
}
