//
// Created by sun on 8/26/16.
//

#include "GameLevelData.h"
#include "GameScene.h"
#include "ImageManager.h"
#include "Timer.h"

GameLevelData::GameLevelData() : cardKind(0),
                                 dKind(1),
                                 sunNum(50),
                                 backgroundImage("interface/background1.jpg"),
                                 LF{ 0, 1, 1, 1, 1, 1 },
                                 canSelectCard(true),
                                 staticCard(true),
                                 showScroll(true),
                                 produceSun(true),
                                 hasShovel(true),
                                 maxSelectedCards(8),
                                 coord(0),
                                 flagNum(0)
{}

void  GameLevelData::loadAccess(GameScene *gameScene)
{
    gameScene->loadAcessFinished();
}

void GameLevelData::startGame(GameScene *gameScene)
{
    initLawnMower(gameScene);
    gameScene->prepareGrowPlants( [gameScene] {
        gameScene->beginBGM();
        gameScene->beginMonitor();
        gameScene->beginCool();
        gameScene->beginSun(25);
        (new Timer(gameScene, 3000/*15000*/, [gameScene] {
            gameScene->beginZombies();
        }))->start();
    });
}

void GameLevelData::initLawnMower(GameScene *gameScene)
{
    for (int i = 0; i < LF.size(); ++i) {
        if (LF[i] == 1)
            gameScene->customSpecial("oLawnCleaner", -1, i);
        else if (LF[i] == 2)
            gameScene->customSpecial("oPoolCleaner", -1, i);
    }
}

void GameLevelData::endGame(GameScene *gameScene)
{

}


GameLevelData_1::GameLevelData_1()
{
    backgroundImage = "interface/background1.jpg";
    backgroundMusic = "qrc:/audio/UraniwaNi.mp3";
    sunNum = 100;
    canSelectCard = true;
    showScroll = true;
    eName = "1";
    cName = tr("Level 1-1");
    pName = { "oPeashooter", "oSnowPea", "oSunflower", "oWallNut" };
    zombieData = { { "oZombie3", 1, 1, {} }, { "oConeheadZombie", 3, 3, {} }, { "oBucketheadZombie", 3, 3, {} } };
    flagNum = 10;
    largeWaveFlag = { 10 };
    flagToSumNum = QPair<QList<int>, QList<int> >({ 3, 5, 9 }, { 1, 2, 3, 15 });
}

GameLevelData *GameLevelDataFactory(const QString &eName)
{
    if (eName == "1")
        return new GameLevelData_1;
    return nullptr;
}
