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
          quitButton      (new MouseEventRectItem     (QRectF(0, 0, 79, 53))),
          usernameText    (new TextItemWithoutBorder  (gMainView->getUsername())),
          backgroundMusic(new QMediaPlayer(this)),
          buttonBleep(new QMediaPlayer(this))
{
    addItem(background);

    quitButton      ->setPen(Qt::NoPen);

    adventureButton ->setCursor(Qt::PointingHandCursor);
    survivalButton  ->setCursor(Qt::PointingHandCursor);
    challengeButton ->setCursor(Qt::PointingHandCursor);
    quitButton      ->setCursor(Qt::PointingHandCursor);

    adventureShadow ->setPos(468, 82);  addItem(adventureShadow);
    adventureButton ->setPos(474, 80);  addItem(adventureButton);
    survivalShadow  ->setPos(476, 208); addItem(survivalShadow);
    survivalButton  ->setPos(474, 203); addItem(survivalButton);
    challengeShadow ->setPos(480, 307); addItem(challengeShadow);
    challengeButton ->setPos(478, 303); addItem(challengeButton);
    quitButton      ->setPos(800, 495); addItem(quitButton);
    woodSign1       ->setPos(20, -8);   addItem(woodSign1);
    woodSign2       ->setPos(23, 126);  addItem(woodSign2);
    woodSign3       ->setPos(34, 179);  addItem(woodSign3);
    zombieHand      ->setPos(262, 264); addItem(zombieHand);
    // Text for username
    usernameText->setParentItem(woodSign1);
    usernameText->setPos(35, 91);
    usernameText->setTextWidth(230);
    usernameText->document()->setDocumentMargin(0);
    usernameText->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    usernameText->setDefaultTextColor(QColor::fromRgb(0xf0c060));
    usernameText->setFont(QFont("Microsoft YaHei", 14, QFont::Bold));

    usernameText->installEventFilter(this);
    usernameText->setTextInteractionFlags(Qt::TextEditorInteraction);

    backgroundMusic->setMedia(QUrl("qrc:/audio/Faster.mp3"));
    buttonBleep->setMedia(QUrl("qrc:/audio/bleep.mp3"));

    connect(backgroundMusic, &QMediaPlayer::stateChanged, [this](QMediaPlayer::State state) {
        if (state == QMediaPlayer::StoppedState)
            backgroundMusic->play();
    });

    connect(adventureButton, &HoverChangedPixmapItem::hoverEntered, [this] { buttonBleep->stop(); buttonBleep->play(); });
    connect(survivalButton, &HoverChangedPixmapItem::hoverEntered, [this] { buttonBleep->stop(); buttonBleep->play(); });
    connect(challengeButton, &HoverChangedPixmapItem::hoverEntered, [this] { buttonBleep->stop(); buttonBleep->play(); });

    QMediaPlayer *loseMusic = new QMediaPlayer(this);
    loseMusic->setMedia(QUrl("qrc:/audio/losemusic.mp3"));

    connect(adventureButton, &HoverChangedPixmapItem::clicked, zombieHand, [this, loseMusic] {
        adventureButton->setCursor(Qt::ArrowCursor);
        survivalButton->setCursor(Qt::ArrowCursor);
        challengeButton->setCursor(Qt::ArrowCursor);
        woodSign3->setCursor(Qt::ArrowCursor);
        adventureButton->setEnabled(false);
        survivalButton->setEnabled(false);
        challengeButton->setEnabled(false);
        woodSign3->setEnabled(false);

        zombieHand->start();
        backgroundMusic->pause();
        loseMusic->play();

    });
    connect(zombieHand, &MoviePixmapItem::finished, [this, loseMusic] {
        (new Timer(this, 2500, [this, loseMusic](){
            loseMusic->stop();
            gMainView->switchToGameScene(QSettings().value("Global/NextLevel", "1").toString());
        }))->start();
    });
    connect(quitButton, &MouseEventRectItem::clicked, [] {
        gMainView->getMainWindow()->close();
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
            return false;
        }
        return false;
    }
    return QGraphicsScene::eventFilter(watched, event);
}

void SelectorScene::loadReady()
{
    // Animation is so UGLY.
    //moveItemWithDuration(woodSign1, QPointF(20, -8), 400, [] {}, QTimeLine::EaseOutCurve);
    //moveItemWithDuration(woodSign2, QPointF(23, 126), 500, [] {}, QTimeLine::EaseOutCurve);
    //moveItemWithDuration(woodSign3, QPointF(34, 179), 600, [] {}, QTimeLine::EaseOutCurve);
    gMainView->getMainWindow()->setWindowTitle(tr("Plants vs. Zombies"));
    backgroundMusic->play();
}
