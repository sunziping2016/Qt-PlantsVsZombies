//
// Created by sun on 8/25/16.
//

#include "SelectorScene.h"
#include "MainView.h"
#include "MouseEventPixmapItem.h"
#include "ImageManager.h"

TextItemWithoutBorder::TextItemWithoutBorder(const QString &text, QGraphicsItem *parent)
        : QGraphicsTextItem(text, parent)
{}

void TextItemWithoutBorder::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem myOption(*option);
    myOption.state &= ~(QStyle::State_Selected | QStyle::State_HasFocus);
    QGraphicsTextItem::paint(painter, &myOption, widget);
}

SelectorScene::SelectorScene()
        : QGraphicsScene(0, 0, 900, 600),
          background      (new QGraphicsPixmapItem    (gImageCache->load("interface/SelectorBackground.png"))),
          adventureShadow (new QGraphicsPixmapItem    (gImageCache->load("interface/SelectorAdventureShadow.png"))),
          adventureButton (new HoverChangedPixmapItem (gImageCache->load("interface/SelectorAdventureButton.png"))),
          survivalShadow  (new QGraphicsPixmapItem    (gImageCache->load("interface/SelectorSurvivalShadow.png"))),
          survivalButton  (new HoverChangedPixmapItem (gImageCache->load("interface/SelectorSurvivalButton.png"))),
          challengeShadow (new QGraphicsPixmapItem    (gImageCache->load("interface/SelectorChallengeShadow.png"))),
          challengeButton (new HoverChangedPixmapItem (gImageCache->load("interface/SelectorChallengeButton.png"))),
          woodSign1       (new QGraphicsPixmapItem    (gImageCache->load("interface/SelectorWoodSign1.png"))),
          woodSign2       (new QGraphicsPixmapItem    (gImageCache->load("interface/SelectorWoodSign2.png"))),
          woodSign3       (new QGraphicsPixmapItem    (gImageCache->load("interface/SelectorWoodSign3.png"))),
          zombieHand      (new MoviePixmapItem        ("interface/SelectorZombieHand.gif")),
          usernameText(new TextItemWithoutBorder(gMainView->getUsername()))
{
    addItem(background);

    adventureButton ->setCursor(Qt::PointingHandCursor);
    survivalButton  ->setCursor(Qt::PointingHandCursor);
    challengeButton ->setCursor(Qt::PointingHandCursor);

    adventureShadow ->setPos(468, 82);  addItem(adventureShadow);
    adventureButton ->setPos(474, 80);  addItem(adventureButton);
    survivalShadow  ->setPos(476, 208); addItem(survivalShadow);
    survivalButton  ->setPos(474, 203); addItem(survivalButton);
    challengeShadow ->setPos(480, 307); addItem(challengeShadow);
    challengeButton ->setPos(478, 303); addItem(challengeButton);
    woodSign1       ->setPos(20, -8);   addItem(woodSign1);
    woodSign2       ->setPos(23, 126);  addItem(woodSign2);
    woodSign3       ->setPos(34, 179);  addItem(woodSign3);
    zombieHand      ->setPos(262, 264); addItem(zombieHand);
    // Text for username
    usernameText->setPos(55, 83);
    usernameText->setTextWidth(230);
    usernameText->document()->setDocumentMargin(0);
    usernameText->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    usernameText->setDefaultTextColor(QColor(0xf0c060));
    usernameText->setFont(QFont("Microsoft YaHei", 14, QFont::Bold));
    addItem(usernameText);

    usernameText->installEventFilter(this);
    usernameText->setTextInteractionFlags(Qt::TextEditorInteraction);

    connect(adventureButton, &HoverChangedPixmapItem::click, zombieHand, [this] {
        adventureButton ->setCursor(Qt::ArrowCursor);
        survivalButton  ->setCursor(Qt::ArrowCursor);
        challengeButton ->setCursor(Qt::ArrowCursor);
        adventureButton ->setEnabled(false);
        survivalButton  ->setEnabled(false);
        challengeButton ->setEnabled(false);

        zombieHand->start();
    });
    connect(zombieHand, &MoviePixmapItem::finished, [this] {
        (new Timer(this, 500, [this](){
            gMainView->switchToGameScene(QSettings().value("Global/NextLevel", "1").toString());
        }))->start();
    });
}

bool SelectorScene::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == usernameText) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                // Save the username
                gMainView->setUsername(usernameText->toPlainText());
                setFocusItem(nullptr);
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }
    return QGraphicsScene::eventFilter(watched, event);
}
