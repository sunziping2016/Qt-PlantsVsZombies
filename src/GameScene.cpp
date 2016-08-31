//
// Created by sun on 8/26/16.
//

#include <QtAlgorithms>
#include <stdlib.h>
#include <math.h>
#include "GameScene.h"
#include "MainView.h"
#include "ImageManager.h"
#include "Timer.h"
#include "Plant.h"
#include "Zombie.h"
#include "GameLevelData.h"
#include "MouseEventPixmapItem.h"
#include "PlantCardItem.h"


GameScene::GameScene(GameLevelData *gameLevelData)
        : QGraphicsScene(0, 0, 1400, 600),
          gameLevelData(gameLevelData),
          background(new QGraphicsPixmapItem(gImageCache->load(gameLevelData->backgroundImage))),
          techText(new QGraphicsTextItem),
          techTextGroup(new QGraphicsItemGroup),
          displayZombiesGroup(new QGraphicsItemGroup),
          menuGroup(new QGraphicsItemGroup),
          sunNumText(new QGraphicsTextItem(QString::number(gameLevelData->sunNum))),
          sunNumGroup(new QGraphicsItemGroup),
          selectCardButtonReset(new MouseEventPixmapItem(gImageCache->load("interface/SelectCardButton.png"))),
          selectCardButtonOkay(new MouseEventPixmapItem(gImageCache->load("interface/SelectCardButton.png"))),
          selectCardTextReset(new QGraphicsTextItem("重选")),
          selectCardTextOkay(new QGraphicsTextItem("开始")),
          selectCardGroup(new QGraphicsItemGroup),
          selectedPlantGroup(new QGraphicsItemGroup),
          movePlantAlpha(new QGraphicsPixmapItem),
          movePlant(new QGraphicsPixmapItem),
          imgGrowSoil(new MoviePixmapItem("interface/GrowSoil.gif")),
          imgGrowSpray(new MoviePixmapItem("interface/GrowSpray.gif")),
          coordinate(gameLevelData->coord),
          choose(0), sunNum(gameLevelData->sunNum)
{
    gMainView->setMouseTracking(true);

    // Process itemsOnScreen event
    connect(gMainView->horizontalScrollBar(), &QScrollBar::valueChanged, [this](int value) {
        for (const auto &i: itemsOnScreen)
            i.first->setX(i.second + scenePos().x());
    });
    // Background
    addItem(background);
    // Information text
    QGraphicsRectItem *techTextBackground = new QGraphicsRectItem(0, 0, 900, 50, techText);
    techTextBackground->setPen(QPen(Qt::NoPen));
    techTextBackground->setBrush(QBrush(QColor::fromRgba(0xE65B432E)));
    techTextGroup->addToGroup(techTextBackground);
    techText->setPos(0, 10);
    techText->setTextWidth(900);
    techText->document()->setDocumentMargin(0);
    techText->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter | Qt::AlignVCenter));
    techText->setFont(QFont("Microsoft YaHei", 16, QFont::Bold));
    techTextGroup->addToGroup(techText);
    techTextGroup->setPos(0, 500);
    techTextGroup->setVisible(false);
    addItemOnScreen(techTextGroup);
    // Zombies on the road
    displayZombiesGroup->setVisible(false);
    addItem(displayZombiesGroup);
    // Menu
    MouseEventPixmapItem *menuBackground = new MouseEventPixmapItem(gImageCache->load("interface/Button.png"));
    menuGroup->addToGroup(menuBackground);
    QGraphicsTextItem *menuText = new QGraphicsTextItem("菜  单");
    menuText->setPos(0, 12);
    menuText->setTextWidth(113);
    menuText->document()->setDocumentMargin(0);
    menuText->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    menuText->setFont(QFont("SimHei", 12, QFont::Bold));
    menuText->setDefaultTextColor(QColor::fromRgb(0x00CB08));
    menuGroup->addToGroup(menuText);
    menuGroup->setPos(787, 0);
    menuGroup->setVisible(false);
    menuGroup->setEnabled(false);
    addItemOnScreen(menuGroup);
    // Sun number
    QGraphicsPixmapItem *sunNumBackground = new QGraphicsPixmapItem(gImageCache->load("interface/SunBack.png"));
    sunNumGroup->addToGroup(sunNumBackground);
    sunNumText->setPos(43, 4);
    sunNumText->setTextWidth(68);
    sunNumText->document()->setDocumentMargin(0);
    sunNumText->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    sunNumText->setFont(QFont("Verdana", 16, QFont::Bold));
    sunNumGroup->addToGroup(sunNumText);
    sunNumGroup->setPos(100, 560);
    sunNumGroup->setVisible(false);
    addItemOnScreen(sunNumGroup);
    // Select Card
    QGraphicsPixmapItem *selectCardBackground = new QGraphicsPixmapItem(
            gImageCache->load("interface/SeedChooser_Background.png"));
    selectCardGroup->addToGroup(selectCardBackground);
    QGraphicsTextItem *selectCardTitle = new QGraphicsTextItem("选择你的卡片");
    selectCardTitle->setPos(0, 8);
    selectCardTitle->setTextWidth(465);
    selectCardTitle->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    selectCardTitle->setFont(QFont("NSimSun", 12, QFont::Bold));
    selectCardTitle->setDefaultTextColor(QColor::fromRgb(0xF0C060));
    selectCardGroup->addToGroup(selectCardTitle);
    selectCardButtonReset->setPos(162, 500);
    selectCardButtonReset->setEnabled(false);
    selectCardGroup->addToGroup(selectCardButtonReset);
    selectCardButtonOkay->setPos(237, 500);
    selectCardButtonOkay->setEnabled(false);
    selectCardGroup->addToGroup(selectCardButtonOkay);
    selectCardTextReset->setPos(162, 510);
    selectCardTextReset->setTextWidth(65);
    selectCardTextReset->document()->setDocumentMargin(0);
    selectCardTextReset->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    selectCardTextReset->setDefaultTextColor(QColor::fromRgb(0x808080));
    selectCardTextReset->setFont(QFont("SimHei", 12, QFont::Bold));
    selectCardGroup->addToGroup(selectCardTextReset);
    selectCardTextOkay->setPos(237, 510);
    selectCardTextOkay->setTextWidth(65);
    selectCardTextOkay->document()->setDocumentMargin(0);
    selectCardTextOkay->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    selectCardTextOkay->setDefaultTextColor(QColor::fromRgb(0x808080));
    selectCardTextOkay->setFont(QFont("SimHei", 12, QFont::Bold));
    selectCardGroup->addToGroup(selectCardTextOkay);
    selectCardGroup->setPos(100, 0);
    selectCardGroup->setHandlesChildEvents(false);
    selectCardGroup->setVisible(false);
    selectCardGroup->setEnabled(false);
    addItemOnScreen(selectCardGroup);
    selectedPlantGroup->setHandlesChildEvents(false);
    addItemOnScreen(selectedPlantGroup);
    movePlantAlpha->setVisible(false);
    movePlantAlpha->setZValue(1);
    addItemOnScreen(movePlantAlpha);
    movePlant->setVisible(false);
    movePlant->setZValue(3);
    addItem(movePlant);
    imgGrowSoil->setVisible(false);
    imgGrowSoil->setZValue(1);
    addItemOnScreen(imgGrowSoil);
    imgGrowSpray->setVisible(false);
    imgGrowSpray->setZValue(1);
    addItemOnScreen(imgGrowSpray);

    // Process ProtoTypes
    for (const auto &eName: gameLevelData->pName)
        plantProtoTypes.insert(eName, PlantFactory(eName));
    for (const auto &eName: gameLevelData->zName)
        zombieProtoTypes.insert(eName, ZombieFactory(eName));

    for(const auto &i: gameLevelData->zombieData) {
        for (int j = 0; j < i.num; ++j)
            zombieArray.push_back(QPair<Zombie *, int>(ZombieFactory(i.ename), i.firstFlag));
        for(auto j: i.flagList)
            if (mustShowAtFlag.find(j) == mustShowAtFlag.end())
                mustShowAtFlag.insert(j, QVector<QString>{i.ename});
            else
                mustShowAtFlag[j].push_back(i.ename);
    }

    qSort(zombieArray.begin(), zombieArray.end(), [](const QPair<Zombie *, int> &a, const QPair<Zombie *, int> &b){
        return a.second < b.second;
    });
}

GameScene::~GameScene()
{
    for (auto i: plantProtoTypes.values())
        delete i;
    for (auto i: zombieProtoTypes.values())
        delete i;
    for (auto i: plantInstances)
        delete i;

    for(const auto &i: zombieArray)
        delete i.first;
    delete gameLevelData;
    disconnect(gMainView->horizontalScrollBar(), &QScrollBar::valueChanged, 0, 0);
}

void GameScene::setInfoText(QString text, const QColor &color)
{
    if (text.isEmpty())
        techTextGroup->setVisible(false);
    else {
        techText->setDefaultTextColor(color);
        techText->setPlainText(text);
        techTextGroup->setVisible(true);
    }
}

void GameScene::loadReady()
{
    if (!gameLevelData->showScroll)
        gMainView->setScenePos(QPointF(115, 0));
    gameLevelData->loadAccess(*this, &GameScene::checkScroll);
}

void GameScene::checkScroll()
{
    if (gameLevelData->showScroll) {
        setInfoText(QSettings().value("Global/Username").toString() + "的房间");
        (new Timer(this, 1000, [this]{
            setInfoText("");
            initDisplayZombies();
            displayZombiesGroup->setVisible(true);
            gMainView->scrollToScenePos(QPointF(500, 0), 1, [this]{
                menuGroup->setVisible(true);
                menuGroup->setEnabled(true);
                if (gameLevelData->canSelectCard) {
                    initPlantCards();
                    sunNumGroup->setVisible(true);
                    selectCardGroup->setVisible(true);
                    selectCardGroup->setEnabled(true);
                    connect(selectCardButtonOkay, &MouseEventPixmapItem::click, [this]{
                        sunNumGroup->setVisible(false);
                        selectCardGroup->setVisible(false);
                        selectCardGroup->setEnabled(false);
                        gMainView->scrollToScenePos(QPointF(115, 0), 1, [this]{
                            displayZombiesGroup->setVisible(false);
                            for (const auto &i: displayZombiesGroup->childItems())
                                dynamic_cast<MoviePixmapItem *>(i)->stop();
                            letsGo();
                        });
                    });
                }
                else {
                    for (auto item: plantProtoTypes.values()) {
                        if (item->canSelect) {
                            selectedPlantArray.push_back(item);
                            if (selectedPlantArray.size() == 8)
                                break;
                        }
                    }
                    (new Timer(this, 1000, [this]{
                        gMainView->scrollToScenePos(QPointF(115, 0), 1, [this]{
                            displayZombiesGroup->setVisible(false);
                            for (const auto &i: displayZombiesGroup->childItems())
                                dynamic_cast<MoviePixmapItem *>(i)->stop();
                            letsGo();
                        });
                    }))->start();
                }
            });
        }))->start();
    }
    else {
        menuGroup->setVisible(true);
        menuGroup->setEnabled(true);
        // TODO: autoSelectCard();
        letsGo();
    }
}

void GameScene::initDisplayZombies()
{
    QVector<double> yPos;
    QVector<Zombie *> zombies;
    for (const auto &i: zombieArray)
        if(i.first->canDisplay) {
            yPos.push_back(qFloor(100 + (double) qrand() / RAND_MAX * 400));
            zombies.push_back(i.first);
        }
    qSort(yPos.begin(), yPos.end());
    std::random_shuffle(zombies.begin(), zombies.end());
    for (int i = 0; i < zombies.size(); ++i) {
        MoviePixmapItem *pixmap = new MoviePixmapItem(zombies[i]->standGif);
        pixmap->start();
        pixmap->setPos(qFloor(1115 + (double) qrand() / RAND_MAX * 200) - pixmap->pixmap().width() * 0.5, yPos[i] - pixmap->pixmap().width() * 0.5);
        displayZombiesGroup->addToGroup(pixmap);
    }

}

void GameScene::addItemOnScreen(QGraphicsItem *item)
{
    addItem(item);
    itemsOnScreen.push_back(QPair<QGraphicsItem *, qreal>(item, item->x()));
    item->setX(item->x() + scenePos().x());
}

void GameScene::setItemOnScreenPos(QGraphicsItem *item, qreal x, qreal y)
{
    for (auto &i: itemsOnScreen)
        if (i.first == item) {
            i.second = x;
            i.first->setPos(x + scenePos().x(), y);
        }
}

void GameScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
    emit mouseMove(mouseEvent);
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
    emit mousePress(mouseEvent);
}

QPointF GameScene::scenePos() const
{
    return gMainView->mapToScene(0, 0);
}

GameLevelData *GameScene::getGameLevelData() const
{
    return gameLevelData;
}


void GameScene::initPlantCards()
{
    QVector<TooltipItem *> tooltipItems;
    int i = 0;
    for (auto item: plantProtoTypes.values()) {
        if (!item->canSelect)
            continue;
        PlantCardItem *plantCardItem = new PlantCardItem(item, true);
        selectCardGroup->addToGroup(plantCardItem);
        plantCardItem->setPos(15 + i % 6 * 72, 40 + i / 6 * 50);
        QString text = "<b>" + item->cName + "</b><br />" +
                       "冷却时间：" + QString::number(item->coolTime) + "秒<br />";
        if (gameLevelData->dKind != 0 && item->night)
            text += "<span style=\"color:#F00\">夜行性的--在白天会睡觉</span><br>";
        text += item->toolTip;
        TooltipItem *tooltipItem = new TooltipItem(text);
        tooltipItem->setVisible(false);
        tooltipItem->setOpacity(0.9);
        tooltipItems.push_back(tooltipItem);
        addItem(tooltipItem);

        connect(plantCardItem, &PlantCardItem::hoverEnter, [tooltipItem](QGraphicsSceneHoverEvent *event) {
            tooltipItem->setPos(event->scenePos() + QPointF(5, 15));
            tooltipItem->setVisible(true);
        });
        connect(plantCardItem, &PlantCardItem::hoverMove, [tooltipItem](QGraphicsSceneHoverEvent *event) {
            tooltipItem->setPos(event->scenePos() + QPointF(5, 15));
        });
        connect(plantCardItem, &PlantCardItem::hoverLeave, [tooltipItem](QGraphicsSceneHoverEvent *event) {
            tooltipItem->setPos(event->scenePos() + QPointF(5, 15));
            tooltipItem->setVisible(false);
        });

        connect(plantCardItem, &PlantCardItem::click, [this, plantCardItem, item] {
            if (!plantCardItem->isChecked()) return;
            int count = selectedPlantGroup->childItems().count();
            if (count >= 8) return;
            plantCardItem->setChecked(false);
            PlantCardItem *selectedPlantCardItem = new PlantCardItem(item);
            selectedPlantGroup->addToGroup(selectedPlantCardItem);
            selectedPlantCardItem->setPos(0, count * 60);
            selectedPlantArray.push_back(item);
            if (count == 0) {
                selectCardTextReset->setDefaultTextColor(QColor::fromRgb(0xf0c060));
                selectCardTextOkay->setDefaultTextColor(QColor::fromRgb(0xf0c060));
                selectCardButtonReset->setEnabled(true);
                selectCardButtonOkay->setEnabled(true);
                selectCardButtonReset->setCursor(Qt::PointingHandCursor);
                selectCardButtonOkay->setCursor(Qt::PointingHandCursor);
            }

            QSharedPointer<QMetaObject::Connection> connection(new QMetaObject::Connection());
            auto deselectFunction = [this, plantCardItem, item, selectedPlantCardItem, connection] {
                plantCardItem->setChecked(true);
                for (auto x: selectedPlantGroup->childItems())
                    if (x != selectedPlantCardItem && x->y() > selectedPlantCardItem->y())
                        x->setY(x->y() - 60);
                selectedPlantCardItem->setParentItem(nullptr);
                selectedPlantArray.remove(selectedPlantArray.indexOf(item));
                disconnect(*connection.data());
                delete selectedPlantCardItem;
                if (selectedPlantGroup->childItems().count() == 0) {
                    selectCardTextReset->setDefaultTextColor(QColor::fromRgb(0x808080));
                    selectCardTextOkay->setDefaultTextColor(QColor::fromRgb(0x808080));
                    selectCardButtonReset->setEnabled(false);
                    selectCardButtonOkay->setEnabled(false);
                    selectCardButtonReset->setCursor(Qt::ArrowCursor);
                    selectCardButtonOkay->setCursor(Qt::ArrowCursor);
                }
            };
            connect(selectedPlantCardItem, &PlantCardItem::click, deselectFunction);
            *connection.data() = connect(selectCardButtonReset, &MouseEventPixmapItem::click, deselectFunction);
        });
        ++i;
    }
    connect(selectCardButtonOkay, &MouseEventPixmapItem::click, [tooltipItems]{
        for (auto item: tooltipItems) {
            item->setParentItem(nullptr);
            delete item;
        }
    });
}

void GameScene::letsGo()
{
    setItemOnScreenPos(sunNumGroup, 105, 0);
    sunNumGroup->setVisible(true);
    for (auto i: selectedPlantGroup->childItems()) {
        delete i;
    }
    plantReady.insert(0, selectedPlantArray.size(), QPair<bool, bool>(false, false));
    for (int i = 0; i < selectedPlantArray.size(); ++i) {
        auto &item = selectedPlantArray[i];
        PlantCardItem *plantCardItem = new PlantCardItem(item);
        plantCardItem->setChecked(false);
        selectedPlantGroup->addToGroup(plantCardItem);
        plantCardItem->setPos(0, i * 60);
        TooltipItem *tooltipItem = new TooltipItem("");
        tooltipItem->setVisible(false);
        tooltipItem->setOpacity(0.9);
        addItem(tooltipItem);

        connect(plantCardItem, &PlantCardItem::hoverEnter, [this, tooltipItem, item, i](QGraphicsSceneHoverEvent *event) {
            if (choose) return;
            QString text = "<b>" + item->cName + "</b><br />" +
                           "冷却时间：" + QString::number(item->coolTime) + "秒<br />";
            text += item->toolTip;
            if (!plantReady[i].first)
                text += "<br><span style=\"color:#f00\">正在重新装填中...</span>";
            if (!plantReady[i].second)
                text += "<br><span style=\"color:#f00\">阳光不足!</span>";
            tooltipItem->setText(text);
            tooltipItem->setPos(event->scenePos() + QPointF(5, 15));
            tooltipItem->setZValue(1);
            tooltipItem->setVisible(true);
        });
        connect(plantCardItem, &PlantCardItem::hoverMove, [this, tooltipItem](QGraphicsSceneHoverEvent *event) {
            if (choose) return;
            tooltipItem->setPos(event->scenePos() + QPointF(5, 15));
        });
        connect(plantCardItem, &PlantCardItem::hoverLeave, [this, tooltipItem](QGraphicsSceneHoverEvent *event) {
            if (choose) return;
            tooltipItem->setPos(event->scenePos() + QPointF(5, 15));
            tooltipItem->setVisible(false);
        });

        selectedPlantGraphicsArray.push_back(QPair<PlantCardItem *, TooltipItem *>(plantCardItem, tooltipItem));
    }
    // All excluded mousePress or click must be triggered from one object to avoid duplicated triggering.
    connect(this, &GameScene::mousePress, [this](QGraphicsSceneMouseEvent *event) {
        if (choose) return;
        for (int i = 0; i < selectedPlantArray.size(); ++i) {
            if (!plantReady[i].first || !plantReady[i].second ||
                !selectedPlantGraphicsArray[i].first->contains(event->scenePos() - selectedPlantGraphicsArray[i].first->scenePos()))
                continue;
            selectedPlantGraphicsArray[i].second->setVisible(false);
            auto &item = selectedPlantArray[i];
            QPixmap staticGif = gImageCache->load(item->staticGif);
            QPointF delta = QPointF(- 0.5 * (item->beAttackedPointL + item->beAttackedPointR), 20 - staticGif.height());
            movePlant->setPixmap(staticGif);
            movePlantAlpha->setPixmap(staticGif);
            movePlant->setPos(event->scenePos() + delta);
            movePlantAlpha->setOpacity(0.4);
            movePlant->setVisible(true);
            QPointF pos = event->scenePos() - scenePos();
            auto xPair = coordinate.choosePlantX(pos.x()), yPair = coordinate.choosePlantY(pos.y());
            if (item->canGrow(*this, xPair.second, yPair.second)) {
                movePlantAlpha->setVisible(true);
                setItemOnScreenPos(movePlantAlpha, xPair.first + item->getDX(),
                                   yPair.first + item->getDY(xPair.second, yPair.second) - item->height);
            }
            else
                movePlantAlpha->setVisible(false);

            choose = 1;
            QSharedPointer<QMetaObject::Connection> connection1(new QMetaObject::Connection()), connection2(
                    new QMetaObject::Connection());
            *connection1.data() = connect(this, &GameScene::mouseMove, [this, delta, item](QGraphicsSceneMouseEvent *e) {
                movePlant->setPos(e->scenePos() + delta);
                QPointF pos = e->scenePos() - scenePos();
                auto xPair = coordinate.choosePlantX(pos.x()), yPair = coordinate.choosePlantY(pos.y());
                if (item->canGrow(*this, xPair.second, yPair.second)) {
                    movePlantAlpha->setVisible(true);
                    setItemOnScreenPos(movePlantAlpha, xPair.first + item->getDX(),
                                       yPair.first + item->getDY(xPair.second, yPair.second) - item->height);
                }
                else
                    movePlantAlpha->setVisible(false);
            });
            *connection2.data() = connect(this, &GameScene::mousePress, [this, i, connection1, connection2, item](QGraphicsSceneMouseEvent *e) {
                choose = 0;
                movePlant->setVisible(false);
                movePlantAlpha->setVisible(false);
                QPointF pos = e->scenePos() - scenePos();
                auto xPair = coordinate.choosePlantX(pos.x()), yPair = coordinate.choosePlantY(pos.y());
                if (item->canGrow(*this, xPair.second, yPair.second)) {
                    MoviePixmapItem *growGif;
                    if (gameLevelData->LF[yPair.second] == 1)
                        growGif = imgGrowSoil;
                    else
                        growGif = imgGrowSpray;
                    setItemOnScreenPos(growGif, xPair.first - 30, yPair.first - 30);
                    growGif->setVisible(true);
                    growGif->start();
                    QSharedPointer<QMetaObject::Connection> connection(new QMetaObject::Connection());
                    *connection.data() = connect(growGif, &MoviePixmapItem::finished, [growGif, connection]{
                        growGif->setVisible(false);
                        growGif->reset();
                        disconnect(*connection.data());
                    });
                    PlantInstance *plantInstance = PlantInstanceFactory(item);
                    plantInstance->birth(*this, xPair.first, yPair.first, xPair.second, yPair.second);
                    plantInstances.push_back(plantInstance);
                    doCoolTime(i);
                    sunNum -= item->sunNum;
                    updateSunNum();
                }
                disconnect(*connection1.data());
                disconnect(*connection2.data());
            });
        }
    });
    gameLevelData->startGame(*this);
    qDebug() << gameLevelData->cName ;
}

void GameScene::beginCool()
{
    for (int i = 0; i < selectedPlantArray.size(); ++i) {
        auto &item = selectedPlantArray[i];
        auto &plantCardItem = selectedPlantGraphicsArray[i].first;
        if (item->coolTime < 7.6) {
            plantCardItem->setPercent(1.0);
            plantReady[i].first = true;
            if (item->sunNum <= sunNum) {
                plantReady[i].second = true;
                plantCardItem->setChecked(true);
            }
            continue;
        }
        doCoolTime(i);
    }
}

void GameScene::autoProduceSun(int sunNum)
{
    MoviePixmapItem *sunGif = new MoviePixmapItem("interface/Sun.gif");
    if (sunNum == 15)
        sunGif->setScale(46.0 / 79.0);
    else if (sunNum != 25)
        sunGif->setScale(100.0 / 79.0);
    double toX = coordinate.getX(static_cast<int>((double)qrand() / RAND_MAX * (coordinate.colCount() - 1) + 1 + 0.5)) + scenePos().x(),
           toY = coordinate.getY(static_cast<int>((double)qrand() / RAND_MAX * (coordinate.rowCount() - 1) + 1 + 0.5));
    sunGif->setPos(toX, -100);
    sunGif->setZValue(2);
    sunGif->setOpacity(0.8);
    sunGif->setCursor(Qt::PointingHandCursor);
    sunGif->start();
    addItem(sunGif);
    QSharedPointer<QTimer *> timer(new QTimer *(nullptr));
    QSharedPointer<QTimeLine *> anim2(new QTimeLine *(nullptr));
    QTimeLine *anim1 = moveItemTo(sunGif, toX, toY - 53, static_cast<int>(toY * 25 + 0.5), [this, sunGif, timer, anim2] {
        (*timer = new Timer(this, 8000, [this, sunGif, anim2] {
            *anim2 = new QTimeLine(500, this);
            (*anim2)->setUpdateInterval(20);
            connect((*anim2), &QTimeLine::valueChanged, [sunGif](qreal x) {
                sunGif->setOpacity((0.0 - 0.8) * x + 0.8);
            });
            connect((*anim2), &QTimeLine::finished, [sunGif] {
                sunGif->setParentItem(nullptr);
                delete sunGif;
            });
            (*anim2)->start();
        }))->start();
    });
    connect(sunGif, &MoviePixmapItem::click, [this, sunGif, sunNum, anim1, anim2, timer] {
        if (choose != 0) return;
        anim1->stop();
        if (*timer != nullptr)
            (*timer)->stop();
        if (*anim2 != nullptr)
            (*anim2)->stop();
        double toX = 100 + scenePos().x(),
                distance = qSqrt((sunGif->x() - toX) * (sunGif->x() - toX) + sunGif->y() * sunGif->y());
        moveItemTo(sunGif, toX, 0, static_cast<int>(distance), [this, sunGif, sunNum]{
            sunGif->setParentItem(nullptr);
            delete sunGif;
            this->sunNum += sunNum;
            updateSunNum();
        });
    });
    (new Timer(this, static_cast<int>(((double)qrand() / RAND_MAX * 3 + 9) * 1000), [this, sunNum] { autoProduceSun(sunNum); }))->start();
}

void GameScene::doCoolTime(int index)
{
    auto &item = selectedPlantArray[index];
    selectedPlantGraphicsArray[index].first->setPercent(0);
    selectedPlantGraphicsArray[index].first->setChecked(false);
    plantReady[index].first = false;
    QTimeLine *anim = new QTimeLine(static_cast<int>(item->coolTime * 1000 + 0.5), this);
    anim->setUpdateInterval(20);
    connect(anim, &QTimeLine::valueChanged, [this, index](qreal x) {
        selectedPlantGraphicsArray[index].first->setPercent(x);
    });
    connect(anim, &QTimeLine::finished, [this, index] {
        plantReady[index].first = true;
        if (plantReady[index].second)
            selectedPlantGraphicsArray[index].first->setChecked(true);
    });
    anim->start();
}

void GameScene::updateSunNum()
{
    sunNumText->setPlainText(QString::number(sunNum));
    for (int i = 0; i < selectedPlantArray.size(); ++i) {
        auto &item = selectedPlantArray[i];
        auto &plantCardItem = selectedPlantGraphicsArray[i].first;
        if (item->sunNum <= sunNum) {
            plantReady[i].second = true;
            if (plantReady[i].first)
                plantCardItem->setChecked(true);
        }
        else {
            plantReady[i].second = false;
            plantCardItem->setChecked(false);
        }
    }
}


