//
// Created by sun on 8/25/16.
//

#include "MenuScene.h"
#include "MainWindow.h"
#include "MouseEventPixmapItem.h"
#include "ImageManager.h"
#include "Timeout.h"

MenuScene::MenuScene()
        : QGraphicsScene(0, 0, 900, 600),
          surface(new QGraphicsPixmapItem(gImageManager->loadPixmap("interface/Surface.jpg"))),
          selectorScreen_Shadow_Adventure(new QGraphicsPixmapItem(gImageManager->loadPixmap("interface/SelectorScreen_Shadow_Adventure.png"))),
          selectorScreenStartAdventure_32(new MouseHoverChangedPixmapItem(gImageManager->loadPixmap("interface/SelectorScreenStartAdventure_32.png"))),
          selectorScreen_Shadow_Survival(new QGraphicsPixmapItem(gImageManager->loadPixmap("interface/SelectorScreen_Shadow_Survival.png"))),
          selectorScreenSurvival_32(new MouseHoverChangedPixmapItem(gImageManager->loadPixmap("interface/SelectorScreenSurvival_32.png"))),
          selectorScreen_Shadow_Challenge(new QGraphicsPixmapItem(gImageManager->loadPixmap("interface/SelectorScreen_Shadow_Challenge.png"))),
          selectorScreenChallenge_32(new MouseHoverChangedPixmapItem(gImageManager->loadPixmap("interface/SelectorScreenChallenges_32.png"))),
          selectorScreen_WoodSign1_32(new QGraphicsPixmapItem(gImageManager->loadPixmap("interface/SelectorScreen_WoodSign1_32.png"))),
          selectorScreen_WoodSign2_32(new MouseHoverChangedPixmapItem(gImageManager->loadPixmap("interface/SelectorScreen_WoodSign2_32.png"))),
          selectorScreen_WoodSign3_32(new QGraphicsPixmapItem(gImageManager->loadPixmap("interface/SelectorScreen_WoodSign3_32.png"))),
          selectorUsername(new QGraphicsTextItem(gMainWindow->getUsername())),
          zombieHand(new MoviePixmapItem("interface/ZombieHand.gif"))
{
    gMainWindow->setMouseTracking(true);

    addItem(surface);
    selectorScreen_Shadow_Adventure->setPos(468, 82);
    addItem(selectorScreen_Shadow_Adventure);
    selectorScreenStartAdventure_32->setPos(474, 80);
    addItem(selectorScreenStartAdventure_32);
    selectorScreen_Shadow_Survival->setPos(476, 208);
    addItem(selectorScreen_Shadow_Survival);
    selectorScreenSurvival_32->setPos(474, 203);
    addItem(selectorScreenSurvival_32);
    selectorScreen_Shadow_Challenge->setPos(480, 307);
    addItem(selectorScreen_Shadow_Challenge);
    selectorScreenChallenge_32->setPos(478, 303);
    addItem(selectorScreenChallenge_32);
    selectorScreen_WoodSign1_32->setPos(20, -8);
    addItem(selectorScreen_WoodSign1_32);
    selectorScreen_WoodSign2_32->setPos(23, 126);
    addItem(selectorScreen_WoodSign2_32);
    selectorScreen_WoodSign3_32->setPos(34, 179);
    addItem(selectorScreen_WoodSign3_32);
    selectorUsername->setPos(55, 83);
    selectorUsername->setTextWidth(230);
    selectorUsername->document()->setDocumentMargin(0);
    selectorUsername->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    selectorUsername->setDefaultTextColor(QColor(0xf0c060));
    selectorUsername->setFont(QFont("Microsoft YaHei", 14, QFont::Bold));
    addItem(selectorUsername);
    zombieHand->setPos(262, 264);
    addItem(zombieHand);

    connect(selectorScreen_WoodSign2_32, &MouseHoverChangedPixmapItem::click, [this]{ askForUsername(); });
    connect(selectorScreenStartAdventure_32, &MouseHoverChangedPixmapItem::click, zombieHand, [this]{ zombieHand->start(); });
    connect(zombieHand, &MoviePixmapItem::finished, [this] {
        (new Timeout(500, [this](){
            gMainWindow->switchToGameScene("1");
            zombieHand->reset();
        }, this))->start();
    });
}

void MenuScene::askForUsername()
{
    bool ok;
    QString text = QInputDialog::getText(0, tr("植物大战僵尸"), tr("请输入您的昵称："), QLineEdit::Normal, gMainWindow->getUsername(),
                                         &ok);
    if (ok && !text.isEmpty()) {
        gMainWindow->setUsername(text);
        selectorUsername->setPlainText(text);
    }
}


