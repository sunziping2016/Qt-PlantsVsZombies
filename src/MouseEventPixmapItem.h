//
// Created by sun on 8/25/16.
//

#ifndef PLANTS_VS_ZOMBIES_MOUSEEVENTPIXMAPITEM_H
#define PLANTS_VS_ZOMBIES_MOUSEEVENTPIXMAPITEM_H

#include <QtWidgets>

class MouseEventPixmapItem: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    MouseEventPixmapItem();
    MouseEventPixmapItem(const QPixmap &image);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:
    void click(QGraphicsSceneMouseEvent *event);
    void hoverEnter(QGraphicsSceneHoverEvent *event);
    void hoverMove(QGraphicsSceneHoverEvent *event);
    void hoverLeave(QGraphicsSceneHoverEvent *event);
};

class HoverChangedPixmapItem: public MouseEventPixmapItem
{
    Q_OBJECT

public:
    HoverChangedPixmapItem(const QPixmap &image);

private:
    QPixmap origImage;
    QPixmap hoverImage;
};

class MoviePixmapItem: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    MoviePixmapItem(const QString &filename);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void click(QGraphicsSceneMouseEvent *event);

    void started();
    void finished();

public slots:
    void start();
    void stop();
    void reset();

private:
    QMovie movie;
};

#endif //PLANTS_VS_ZOMBIES_MOUSEEVENTPIXMAPITEM_H
