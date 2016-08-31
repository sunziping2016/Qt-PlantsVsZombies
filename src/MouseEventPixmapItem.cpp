//
// Created by sun on 8/26/16.
//
#include "MouseEventPixmapItem.h"

MouseEventPixmapItem::MouseEventPixmapItem()
{}

MouseEventPixmapItem::MouseEventPixmapItem(const QPixmap &image) : QGraphicsPixmapItem(image)
{}

void MouseEventPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit click(event);
}

void MouseEventPixmapItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    emit hoverEnter(event);
}

void MouseEventPixmapItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    emit hoverLeave(event);
}

void MouseEventPixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    emit hoverMove(event);
}


HoverChangedPixmapItem::HoverChangedPixmapItem(const QPixmap &image) : origImage(image.copy(0, 0, image.width(), image.height() / 2)),
                                                                                 hoverImage(image.copy(0, image.height() / 2, image.width(), image.height() / 2))
{
    setPixmap(origImage);
    setAcceptHoverEvents(true);
    connect(this, &HoverChangedPixmapItem::hoverEnter, [this]{ setPixmap(hoverImage); });
    connect(this, &HoverChangedPixmapItem::hoverLeave, [this]{ setPixmap(origImage); });
}

MoviePixmapItem::MoviePixmapItem(const QString &filename) : movie("images/" + filename)
{
    movie.jumpToFrame(0);
    setPixmap(movie.currentPixmap());
    connect(&movie, &QMovie::frameChanged, [this](int i){ setPixmap(movie.currentPixmap()); });
    connect(&movie, &QMovie::started, [this]{ emit started(); });
    connect(&movie, &QMovie::finished, [this]{ emit finished(); });
}

void MoviePixmapItem::start()
{
    movie.start();
}

void MoviePixmapItem::stop()
{
    movie.stop();
}

void MoviePixmapItem::reset()
{
    movie.jumpToFrame(0);
}

void MoviePixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit click(event);
}


