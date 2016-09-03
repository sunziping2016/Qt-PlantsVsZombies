//
// Created by sun on 8/27/16.
//

#ifndef PLANTS_VS_ZOMBIES_PLANTCARD_H
#define PLANTS_VS_ZOMBIES_PLANTCARD_H


#include <QtWidgets>
#include "MouseEventPixmapItem.h"

class Plant;

class TooltipItem: public QGraphicsRectItem
{
public:
    TooltipItem(const QString &text);
    void setText(const QString &text);
private:
    QGraphicsTextItem *tooltipText;
};

class PlantCardItem: public MouseEventPixmapItem
{
    Q_OBJECT

public:
    PlantCardItem(const Plant *plant, bool smaller = false);
    void setChecked(bool newchecked);
    bool isChecked() const;

    void setPercent(double value);
    void updatePixmap();
private:
    bool checked;
    double percent;
    int lowestHeight, highestHeight;
    QGraphicsPixmapItem *overlayImage;
    QPixmap checkedImage, uncheckedImage;
};


#endif //PLANTS_VS_ZOMBIES_PLANTCARD_H
