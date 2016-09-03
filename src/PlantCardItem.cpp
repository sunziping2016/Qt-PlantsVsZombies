//
// Created by sun on 8/27/16.
//

#include "PlantCardItem.h"
#include "ImageManager.h"
#include "Plant.h"

PlantCardItem::PlantCardItem(const Plant *plant, bool smaller) : checked(true), percent(0), overlayImage(new QGraphicsPixmapItem)
{
    setCursor(Qt::PointingHandCursor);
    QPixmap image = gImageCache->load(plant->cardGif);
    checkedImage = image.copy(0, 0, image.width(), image.height() / 2);
    uncheckedImage = image.copy(0, image.height() / 2, image.width(), image.height() / 2);
    if (!smaller) {
        QPainter p(&uncheckedImage);
        p.setBrush(QBrush(QColor::fromRgba(0x80000000)));
        p.setPen(Qt::NoPen);
        QRegion region = QRegion(uncheckedImage.mask());
        QRect rect = region.boundingRect();
        lowestHeight = rect.y();
        highestHeight = rect.y() + rect.height();
        p.setClipRegion(region);
        p.drawRect(0, 0, uncheckedImage.width(), uncheckedImage.height());
    }
    setPixmap(checkedImage);
    overlayImage->setVisible(false);
    overlayImage->setOpacity(0.4);
    overlayImage->setParentItem(this);
    QGraphicsSimpleTextItem *sunNum = new QGraphicsSimpleTextItem(QString::number(plant->sunNum));
    sunNum->setFont(QFont("Times New Roman", 16));
    QSizeF sunNumSize = sunNum->boundingRect().size();
    sunNum->setPos(checkedImage.width() - sunNumSize.width() - 4, checkedImage.height() - sunNumSize.height());
    sunNum->setParentItem(this);
    setHandlesChildEvents(true);
    if (smaller)
        setScale(0.7);
}

void PlantCardItem::setChecked(bool newchecked)
{
    if (newchecked == checked)
        return;
    checked = newchecked;
    updatePixmap();
    if (checked)
        setCursor(Qt::PointingHandCursor);
    else
        setCursor(Qt::ArrowCursor);
}

bool PlantCardItem::isChecked() const
{
    return checked;
}

void PlantCardItem::setPercent(double value)
{
    percent = value;
    updatePixmap();
}

void PlantCardItem::updatePixmap()
{
    if (checked) {
        overlayImage->setVisible(false);
        setPixmap(checkedImage);
    }
    else {
        int height = static_cast<int>((highestHeight - lowestHeight) * percent + lowestHeight + 0.5);
        if (height) {
            if (height != overlayImage->pixmap().height()) {
                int y = checkedImage.height() - height;
                overlayImage->setPixmap(checkedImage.copy(0, y, checkedImage.width(), height));
                overlayImage->setPos(0, y);
            }
            overlayImage->setVisible(true);
        }
        else
            overlayImage->setVisible(false);
        setPixmap(uncheckedImage);
    }
}


TooltipItem::TooltipItem(const QString &text)
        : tooltipText(new QGraphicsTextItem)
{
    tooltipText->setTextWidth(180);
    tooltipText->setHtml(text);
    tooltipText->setParentItem(this);
    setRect(tooltipText->boundingRect());
    setPen(QPen(Qt::black));
    setBrush(QColor::fromRgb(0xf0f0d0));
}

void TooltipItem::setText(const QString &text)
{
    tooltipText->setHtml(text);
    setRect(tooltipText->boundingRect());
}


