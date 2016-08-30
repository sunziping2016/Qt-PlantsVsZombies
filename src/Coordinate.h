//
// Created by sun on 8/29/16.
//

#ifndef PLANTS_VS_ZOMBIES_COORDINATE_H
#define PLANTS_VS_ZOMBIES_COORDINATE_H


#include <QtCore>

int truncBetween(int value, int low, int high);

class Coordinate
{
public:
    Coordinate(int cood = 0);
    int getCol(double x) const;
    int getRow(double y) const;
    double getX(int c) const;
    double getY(int r) const;
    QPair<double, int> choosePlantX(double x) const;
    QPair<double, int> choosePlantY(double y) const;
    int rowCount() const;
    int colCount() const;

private:
    int row, col;
    QPair<QVector<double>, QVector<int> > x2c, y2r;
    QPair<QVector<int>, QVector<double> > c2x, r2y;
};


#endif //PLANTS_VS_ZOMBIES_COORDINATE_H
