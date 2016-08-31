//
// Created by sun on 8/27/16.
//

#include "PlantCardItem.h"
#include "ImageManager.h"
#include "Plant.h"

PlantCardItem::PlantCardItem(const Plant *plant, bool smaller) : checked(true), percent(0), overlayImage(new QGraphicsPixmapItem)
{
    QPixmap image = gImageCache->load(plant->cardGif);
    checkedImage = image.copy(0, 0, image.width(), image.height() / 2);
    uncheckedImage = image.copy(0, image.height() / 2, image.width(), image.height() / 2);
    if (smaller) {
        checkedImage = checkedImage.scaled(70, 42);
        uncheckedImage = uncheckedImage.scaled(70, 42);
        lowestHeight = highestHeight = 0;
    }
    else {
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
    QGraphicsTextItem *sunNum = new QGraphicsTextItem(QString::number(plant->sunNum));
    sunNum->setTextWidth(30);
    sunNum->document()->setDefaultTextOption(QTextOption(Qt::AlignRight));
    if (smaller) {
        sunNum->setPos(40, 22);
        sunNum->setFont(QFont("Times New Roman", 10));
    }
    else {
        sunNum->setPos(60, 35);
        sunNum->setFont(QFont("Times New Roman", 14));
    }
    sunNum->setParentItem(this);
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
    background = new QGraphicsRectItem(tooltipText->boundingRect());
    background->setBrush(QBrush(QColor::fromRgb(0xf0f0d0)));
    background->setPen(QPen(Qt::black));
    addToGroup(background);
    addToGroup(tooltipText);
}

void TooltipItem::setText(const QString &text)
{
    tooltipText->setHtml(text);
    background->setRect(tooltipText->boundingRect());
}


