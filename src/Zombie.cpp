//
// Created by sun on 8/26/16.
//

#include "Zombie.h"

Zombie::Zombie(const QString &ename, const QString &cname, bool canDisplay, const QString &cardImage, const QString &staticImage,
               const QString &gifImage, const QString &standGif) : ename(ename), cname(cname), canDisplay(canDisplay), cardImage(cardImage),
                                          staticImage(staticImage), gifImage(gifImage), standGif(standGif)
{ }

Zombie1::Zombie1() : Zombie("Zombie", "领带僵尸", true, "Card/Zombies/Zombie.png", "Zombies/Zombie/0.gif", "Zombies/Zombie/Zombie.gif", "Zombies/Zombie/1.gif")
{}

Zombie *ZombieFactory(const QString &ename)
{
    if (ename == "Zombie")
        return new Zombie1;
    return nullptr;
}

