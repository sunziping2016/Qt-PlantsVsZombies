//
// Created by sun on 8/26/16.
//
#include "MouseEventPixmapItem.h"

MouseEventRectItem::MouseEventRectItem()
{
    setAcceptHoverEvents(true);
}

MouseEventRectItem::MouseEventRectItem(const QRectF &rect) : QGraphicsRectItem(rect)
{
    setAcceptHoverEvents(true);
}

void MouseEventRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(event);
}

void MouseEventRectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    emit hoverEntered(event);
}

void MouseEventRectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    emit hoverLeft(event);
}

void MouseEventRectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    emit hoverMoved(event);
}


MouseEventPixmapItem::MouseEventPixmapItem()
{
    setAcceptHoverEvents(true);
}

MouseEventPixmapItem::MouseEventPixmapItem(const QPixmap &image) : QGraphicsPixmapItem(image)
{
    setAcceptHoverEvents(true);
}

void MouseEventPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(event);
}

void MouseEventPixmapItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    emit hoverEntered(event);
}

void MouseEventPixmapItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    emit hoverLeft(event);
}

void MouseEventPixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    emit hoverMoved(event);
}


HoverChangedPixmapItem::HoverChangedPixmapItem(const QPixmap &image) : origImage(image.copy(0, 0, image.width(), image.height() / 2)),
                                                                                 hoverImage(image.copy(0, image.height() / 2, image.width(), image.height() / 2))
{
    setPixmap(origImage);
    setAcceptHoverEvents(true);
    connect(this, &HoverChangedPixmapItem::hoverEntered, [this] { setPixmap(hoverImage); });
    connect(this, &HoverChangedPixmapItem::hoverLeft, [this] { setPixmap(origImage); });
}

MoviePixmapItem::MoviePixmapItem(const QString &filename)
        : movie(nullptr)
{
    setMovie(filename);
}

MoviePixmapItem::MoviePixmapItem()
        : movie(nullptr)
{}

MoviePixmapItem::~MoviePixmapItem()
{
    if (movie) {
        if (movie->state() == QMovie::Running)
            movie->stop();
        delete movie;
    }
}

void MoviePixmapItem::setMovie(const QString &filename)
{
    if (movie) {
        movie->stop();
        delete movie;
    }
    movie = new QMovie(":/images/" + filename);
    movie->jumpToFrame(0);
    setPixmap(movie->currentPixmap());
    connect(movie, &QMovie::frameChanged, [this](int i){
        setPixmap(movie->currentPixmap());
        if (i == 0)
            emit loopStarted();
    });
    connect(movie, &QMovie::finished, [this]{ emit finished(); });
}

void MoviePixmapItem::start()
{
    movie->start();
}

void MoviePixmapItem::stop()
{
    movie->stop();
}

void MoviePixmapItem::reset()
{
    movie->jumpToFrame(0);
}

void MoviePixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit click(event);
}

void MoviePixmapItem::setMovieOnNewLoop(const QString &filename, std::function<void(void)> functor)
{
    QSharedPointer<QMetaObject::Connection> connection(new QMetaObject::Connection);
    *connection = QObject::connect(this, &MoviePixmapItem::loopStarted, [this, connection, filename, functor] {
        setMovie(filename);
        QObject::disconnect(*connection);
        start();
        functor();
    });
}
