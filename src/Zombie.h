//
// Created by sun on 8/26/16.
//

#ifndef PLANTS_VS_ZOMBIES_ZOMBIE_H
#define PLANTS_VS_ZOMBIES_ZOMBIE_H


#include <QtCore>

class Zombie
{
public:
    Zombie(const QString &ename, const QString &cname, bool canDisplay, const QString &cardImage, const QString &staticImage,
           const QString &gifImage, const QString &standGif);

    QString ename, cname;
    bool canDisplay;
    QString cardImage;
    QString staticImage;
    QString gifImage;
    QString standGif;
};

class Zombie1: public Zombie
{
public:
    Zombie1();
};


Zombie *ZombieFactory(const QString &ename);

#endif //PLANTS_VS_ZOMBIES_ZOMBIE_H
