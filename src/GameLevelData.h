//
// Created by sun on 8/26/16.
//

#ifndef PLANTS_VS_ZOMBIES_GAMELEVELDATA_H
#define PLANTS_VS_ZOMBIES_GAMELEVELDATA_H

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
    QList<int> flagList;
};

class GameLevelData
{
    Q_DECLARE_TR_FUNCTIONS(GameLevelData)
public:
    GameLevelData();
    virtual ~GameLevelData() {}

    QString eName, cName;

    QList<QString> pName, zName;
    int cardKind;           // 0: plant     1: zombies
    int dKind;
    int sunNum;

    QString backgroundImage;
    QList<int> LF;
    bool canSelectCard, staticCard, showScroll, produceSun;
    int maxSelectedCards;
    int coord;

    int flagNum;

    QList<ZombieData> zombieData;

    virtual void loadAccess(GameScene *gameScene);
    virtual void initLawnMower(GameScene *gameScene);
    virtual void startGame(GameScene *gameScene);
};

class GameLevelData_1: public GameLevelData
{
    Q_DECLARE_TR_FUNCTIONS(GameLevelData_1)
public:
    GameLevelData_1();
};

GameLevelData * GameLevelDataFactory(const QString &eName);

#endif //PLANTS_VS_ZOMBIES_GAMELEVEL_H
