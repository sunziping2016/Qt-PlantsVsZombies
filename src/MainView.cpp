//
// Created by sun on 8/25/16.
//

#include "MainView.h"
#include "GameLevelData.h"
#include "SelectorScene.h"
#include "GameScene.h"
#include "AspectRatioLayout.h"

MainView *gMainView;

MainView::MainView(MainWindow &mainWindow)
        : width(900), height(600),
          usernameSettingEntry("Global/Username"),
          gameScene(nullptr),
          mainWindow(mainWindow)
{
    gMainView = this;

    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::TextAntialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);

    setFrameStyle(0);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMinimumSize(width, height);
    // Set up username
    if (getUsername().isEmpty()) {
        QString username = qgetenv("USER"); // Linux or Mac
        if (username.isEmpty())
            username = qgetenv("USERNAME"); // Windows
        if (username.isEmpty())
            username = tr("游客");
        setUsername(username);
    }

    selectorScene = new SelectorScene; // Requires username
}

MainView::~MainView()
{
    delete selectorScene;
    if (gameScene)
        delete gameScene;
}

QString MainView::getUsername() const
{
    return QSettings().value(usernameSettingEntry, "").toString();
}

void MainView::setUsername(const QString &username)
{
    return QSettings().setValue(usernameSettingEntry, username);
}

void MainView::setScenePos(QPointF pos)
{
    QPoint viewPos = matrix().map(pos).toPoint();
    horizontalScrollBar()->setValue(viewPos.x());
    verticalScrollBar()->setValue(viewPos.y());
}

void MainView::switchToGameScene(const QString &eName)
{
    GameScene *newGameScene = new GameScene(GameLevelDataFactory(eName));
    setScene(newGameScene);
    if (gameScene)
        delete gameScene;
    gameScene = newGameScene;
    gameScene->loadReady();
}

void MainView::switchToMenuScene()
{
    setScene(selectorScene);
}

void MainView::resizeEvent(QResizeEvent *event)
{
    // `fitInView` has a bug causing extra margin.
    // see "https://bugreports.qt.io/browse/QTBUG-11945"
    QRectF viewRect = frameRect();
    QTransform trans;
    trans.scale(viewRect.width() / width, viewRect.height() / height);
    setTransform(trans);
}

void MainView::wheelEvent(QWheelEvent *event)
{
    QApplication::sendEvent(scene(), event);
}

void MainView::keyPressEvent(QKeyEvent *event)
{
    QApplication::sendEvent(scene(), event);
}

MainWindow::MainWindow()
    : fullScreenSettingEntry("UI/FullScreen"),
      mainView(new MainView(*this)),
      fullScreenAction(new QAction)
{
    // Layout
    QWidget *centralWidget = new QWidget;
    QLayout *layout = new AspectRatioLayout;
    layout->addWidget(mainView);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    // Full Screen action triggered with "F11"
    fullScreenAction->setCheckable(true);
    fullScreenAction->setShortcut(Qt::Key_F11);
    addAction(fullScreenAction);
    connect(fullScreenAction, &QAction::toggled, [this] (bool checked) {
        if (checked)
            setWindowState(Qt::WindowFullScreen);
        else
            setWindowState(Qt::WindowNoState);
        QSettings().setValue(fullScreenSettingEntry, checked);
    });
    fullScreenAction->setChecked(QSettings().value(fullScreenSettingEntry, false).toBool());
    // Set background color to black
    setPalette(Qt::black);
    setAutoFillBackground(true);

    adjustSize();
}

QAction *MainWindow::getFullScreenAction() const
{
    return fullScreenAction;
}