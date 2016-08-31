//
// Created by sun on 8/31/16.
//

#ifndef PLANTS_VS_ZOMBIES_ASPECTRATIOLAYOUT_H
#define PLANTS_VS_ZOMBIES_ASPECTRATIOLAYOUT_H

#include <QtWidgets>

class AspectRatioLayout: public QLayout
{
public:
    explicit AspectRatioLayout(QWidget *parent = nullptr);
    virtual ~AspectRatioLayout();

    virtual void addItem(QLayoutItem *item) override;
    virtual QLayoutItem *itemAt(int index) const override;
    virtual QLayoutItem *takeAt(int index) override;
    virtual int count() const override;

    virtual QSize minimumSize() const override;
    virtual QSize sizeHint() const override;
    virtual void setGeometry(const QRect &rect) override;
    virtual Qt::Orientations expandingDirections() const override;

private:
    QLayoutItem *item;
};

#endif //PLANTS_VS_ZOMBIES_ASPECTRATIOLAYOUT_H
