//
// Created by sun on 8/26/16.
//

#include "Zombie.h"

Zombie::Zombie()
    : hp(270), level(1),
      canSelect(true), canDisplay(true),
      beAttackedPointL(82), beAttackedPointR(156),
      breakPoint(90), sunNum(0), coolTime(0)
{}

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
{}


Zombie *ZombieFactory(const QString &ename)
{
    if (ename == "oZombie")
        return new Zombie1;
    return nullptr;
}

ZombieInstance *ZombieInstanceFactory(const Zombie *zombie)
{
    return new ZombieInstance(zombie);
}



