//
// Created by sun on 8/26/16.
//

#include "GameLevelData.h"
#include "GameScene.h"
#include "ImageManager.h"
#include "Timer.h"

ZombieData::ZombieData(const QString &ename, int num, int firstFlag, std::initializer_list<int> flagList)
        : ename(ename), num(num), firstFlag(firstFlag), flagList(flagList)
{}

ZombieData::ZombieData(const ZombieData &orig) : ename(orig.ename),
                                                 num(orig.num),
                                                 firstFlag(orig.firstFlag),
                                                 flagList(orig.flagList)
{}

GameLevelData::GameLevelData() : cardKind(0),
                                 dKind(1),
                                 sunNum(50),
                                 backgroundImage("interface/background1.jpg"),
                                 LF{ 0, 1, 1, 1, 1, 1 },
                                 canSelectCard(true),
                                 staticCard(true),
                                 showScroll(true),
                                 produceSun(true),
                                 maxSelectedCards(8),
                                 coord(0),
                                 flagNum(0),
{}

void  GameLevelData::loadAccess(GameScene *gameScene)
{
    gameScene->loadAcessFinished();
}

void GameLevelData::startGame(GameScene *gameScene)
{
    QPixmap prepareGrowPlants = gImageCache->load("interface/PrepareGrowPlants.png");
    QGraphicsPixmapItem *imgPrepare = new QGraphicsPixmapItem(prepareGrowPlants.copy(0, 0, 255, 108)),
            *imgGrow    = new QGraphicsPixmapItem(prepareGrowPlants.copy(0, 108, 255, 108)),
            *imgPlants  = new QGraphicsPixmapItem(prepareGrowPlants.copy(0, 216, 255, 108));
    imgPrepare ->setPos(373, 246);
    imgGrow    ->setPos(373, 246);
    imgPlants  ->setPos(373, 246);
    imgPrepare ->setZValue(1);
    imgGrow    ->setZValue(1);
    imgPlants  ->setZValue(1);
    imgPrepare ->setVisible(false);
    imgGrow    ->setVisible(false);
    imgPlants  ->setVisible(false);
    gameScene->addItem(imgPrepare);
    gameScene->addItem(imgGrow);
    gameScene->addItem(imgPlants);
    initLawnMower(gameScene);
    // TODO: haveFog
    imgPrepare->setVisible(true);
    (new Timer(gameScene, 600, [gameScene, imgPrepare, imgGrow, imgPlants] {
        imgPrepare->setVisible(false);
        imgGrow->setVisible(true);
        (new Timer(gameScene, 400, [gameScene, imgGrow, imgPlants] {
            imgGrow->setVisible(false);
            imgPlants->setVisible(true);
            (new Timer(gameScene, 1000, [gameScene, imgPlants] {
                imgPlants->setVisible(false);
                gameScene->beginCool();
                gameScene->autoProduceSun(25);
                (new Timer(gameScene, 15000, [gameScene] {
                    // TODO: flag
                }))->start();
            }))->start();
        }))->start();
    }))->start();
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


GameLevelData_1::GameLevelData_1()
{
    backgroundImage = "interface/background1.jpg";
    sunNum = 325;
    canSelectCard = false;
    showScroll = false;
    eName = "1";
    cName = tr("Level 1-1");
    pName = { "oPeashooter", "oSnowPea", "oSunflower", "oWallNut" };
    zombieData.push_back(ZombieData("Zombie", 5, 1));
    flagNum = 10;
}

GameLevelData *GameLevelDataFactory(const QString &eName)
{
    if (eName == "1")
        return new GameLevelData_1;
    return nullptr;
}
