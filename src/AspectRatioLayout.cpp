//
// Created by sun on 8/31/16.
//

#include "AspectRatioLayout.h"

AspectRatioLayout::AspectRatioLayout(QWidget *parent)
        : QLayout(parent), item(nullptr)
{}

AspectRatioLayout::~AspectRatioLayout()
{
    if (item)
        delete item;
}

void AspectRatioLayout::addItem(QLayoutItem *item)
{
    if (this->item)
        qWarning("AspectRatioLayout: Attempting to add more items.");
    this->item = item;
}

QLayoutItem *AspectRatioLayout::itemAt(int index) const
{
    if (index == 0)
        return item;
    return nullptr;
}

QLayoutItem *AspectRatioLayout::takeAt(int index)
{
    if (index == 0) {
        QLayoutItem *temp = item;
        item = nullptr;
        return temp;
    }
    return nullptr;
}

int AspectRatioLayout::count() const
{
    if (item)
        return 1;
    return 0;
}

QSize AspectRatioLayout::minimumSize() const
{
    if (item)
        return item->minimumSize();
    return QSize();
}

QSize AspectRatioLayout::sizeHint() const
{
    if (item)
        return item->sizeHint();
    return QSize();
}

void AspectRatioLayout::setGeometry(const QRect &rect) {
    QLayout::setGeometry(rect);

    if (!item) return;

    auto height = rect.height();

    QSize hint = item->sizeHint();
    qreal aspectRatio = 0;
    if (hint.width() && hint.height())
        aspectRatio = static_cast<qreal>(hint.width()) / hint.height();

    auto currentWidth = qRound(rect.height() * aspectRatio);
    if (currentWidth <= rect.width()) {
        int offset = (rect.width() - currentWidth) / 2;
        item->setGeometry(QRect(rect.x() + offset, rect.y(), currentWidth, rect.height()));
    } else {
        int currentHeight = qRound(rect.width() / aspectRatio);
        int offset = (height - currentHeight) / 2;
        item->setGeometry(QRect(rect.x(), rect.y() + offset, rect.width(), currentHeight));
    }
}

Qt::Orientations AspectRatioLayout::expandingDirections() const
{
    return Qt::Orientations(0);
}
