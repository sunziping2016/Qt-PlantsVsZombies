//
// Created by sun on 8/25/16.
//

#ifndef PLANTS_VS_ZOMBIES_MENUSCENE_H
#define PLANTS_VS_ZOMBIES_MENUSCENE_H

#include <QtWidgets>

class MouseHoverChangedPixmapItem;
class MoviePixmapItem;

class MenuScene: public QGraphicsScene
{
    Q_OBJECT

public:
    MenuScene();

public slots:
    void askForUsername();

private:
    QGraphicsPixmapItem *surface;
    QGraphicsPixmapItem *selectorScreen_Shadow_Adventure;
    MouseHoverChangedPixmapItem *selectorScreenStartAdventure_32;
    QGraphicsPixmapItem *selectorScreen_Shadow_Survival;
    MouseHoverChangedPixmapItem *selectorScreenSurvival_32;
    QGraphicsPixmapItem *selectorScreen_Shadow_Challenge;
    MouseHoverChangedPixmapItem *selectorScreenChallenge_32;
    QGraphicsPixmapItem *selectorScreen_WoodSign1_32;
    MouseHoverChangedPixmapItem *selectorScreen_WoodSign2_32;
    QGraphicsPixmapItem *selectorScreen_WoodSign3_32;
    QGraphicsTextItem *selectorUsername;
    MoviePixmapItem *zombieHand;
};

#endif //PLANTS_VS_ZOMBIES_MENUSCENE_H
