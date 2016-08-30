//
// Created by sun on 8/29/16.
//

#include "Coordinate.h"

Coordinate::Coordinate(int cood) : col(9)
{
    if (cood == 0) {
        row = 5;
        x2c = QPair<QVector<double>, QVector<int> >(
                { -50, 100, 140, 220, 295, 379, 460, 540, 625, 695, 775, 855, 935 },
                {  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11 });
        c2x = QPair<QVector<int>, QVector<double> >(
                {  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11 },
                { -50, 100, 140, 187, 267, 347, 427, 507, 587, 667, 747, 827, 865, 950 });
        y2r = QPair<QVector<double>, QVector<int> >(
                {  86, 181, 281, 386, 476 },
                {   0,   1,   2,   3,   4,   5 });
        r2y = QPair<QVector<int>, QVector<double> >(
                {   0,   1,   2,   3,   4,   5 },
                {  75, 175, 270, 380, 470, 575 });
    }
    else {
        row = 6;
    }
}

int Coordinate::getCol(double x) const
{
    return x2c.second[qLowerBound(x2c.first, x) - x2c.first.begin()];
}

int Coordinate::getRow(double y) const
{
    return y2r.second[qLowerBound(y2r.first, y) - y2r.first.begin()];
}

double Coordinate::getX(int c) const
{
    return c2x.second[qBinaryFind(c2x.first, c) - c2x.first.begin()];
}

double Coordinate::getY(int r) const
{
    return r2y.second[qBinaryFind(r2y.first, r) - r2y.first.begin()];
}

QPair<double, int> Coordinate::choosePlantX(double x) const
{
    int c = getCol(x);
    return QPair<double, int>(getX(c), c);
}

QPair<double, int> Coordinate::choosePlantY(double y) const
{
    int r = getRow(y);
    return QPair<double, int>(getY(r), r);
}

int truncBetween(int value, int low, int high)
{
    if (value < low)
        return low;
    if (value > high)
        return  high;
    return value;
}

int Coordinate::rowCount() const
{
    return row;
}

int Coordinate::colCount() const
{
    return col;
}