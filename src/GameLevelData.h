//
// Created by sun on 8/26/16.
//

#ifndef PLANTS_VS_ZOMBIES_GAMELEVEL_H
#define PLANTS_VS_ZOMBIES_GAMELEVEL_H

#include <QtCore>

class GameScene;

class ZombieData
{
public:
    ZombieData(const QString &ename = "", int num = 0, int firstFlag = 0, std::initializer_list<int> flagList = {});
    ZombieData(const ZombieData &orig);

    QString ename;
    int num;
    int firstFlag;
    QVector<int> flagList;
};

class GameLevelData
{
public:
    GameLevelData();
    virtual ~GameLevelData() {}

    QString eName, cName;

    QVector<QString> pName, zName;
    int cardKind;           // 0: plant     1: zombies
    int dKind;
    int sunNum;

    QString backgroundImage;
    QVector<int> LF;
    bool canSelectCard, staticCard, showScroll, produceSun;
    int coord;

    QVector<ZombieData> zombieData;

    virtual void loadAccess(GameScene &gameScene, void (GameScene::*callback)());
    virtual void initLawnMower(GameScene &gameScene) {}
    virtual void startGame(GameScene &gameScene);
};

class GameLevelData_1: public GameLevelData
{
public:
    GameLevelData_1();
};

GameLevelData * GameLevelDataFactory(const QString &eName);

#endif //PLANTS_VS_ZOMBIES_GAMELEVEL_H
