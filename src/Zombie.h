//
// Created by sun on 8/26/16.
//

#ifndef PLANTS_VS_ZOMBIES_ZOMBIE_H
#define PLANTS_VS_ZOMBIES_ZOMBIE_H


#include <QtCore>

class Zombie
{
    Q_DECLARE_TR_FUNCTIONS(Zombie)
public:
    Zombie();

    QString eName, cName;

    int width, height;

    int hp, level;
    bool canSelect, canDisplay;

    QString cardGif, staticGif, normalGif, attackGif, lostHeadGif,
            lostHeadAttackGif, headGif, dieGif, boomDieGif, standGif;

    int beAttackedPointL, beAttackedPointR;
    int breakPoint, sunNum;
    qreal coolTime;
};

class Zombie1: public Zombie
{
    Q_DECLARE_TR_FUNCTIONS(Zombie1)
public:
    Zombie1();
};

class ZombieInstance
{
public:
    ZombieInstance(const Zombie *zombie);

private:
    const Zombie *zombieProtoType;
};

Zombie *ZombieFactory(const QString &ename);
ZombieInstance *ZombieInstanceFactory(const Zombie *zombie);

#endif //PLANTS_VS_ZOMBIES_ZOMBIE_H
