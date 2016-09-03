//
// Created by sun on 8/26/16.
//

#ifndef PLANTS_VS_ZOMBIES_TIMEER_H
#define PLANTS_VS_ZOMBIES_TIMEER_H


#include <QtCore>

// Just for convenience
class Timer: public QTimer
{
public:
    Timer(QObject *parent, int timeout, std::function<void(void)> functor);
};

class TimeLine: public QTimeLine
{
public:
    TimeLine(QObject *parent, int duration, int interval, std::function<void(qreal)> onChanged, std::function<void(void)> onFinished = [] {}, CurveShape shape = EaseInOutCurve);
};

#endif //PLANTS_VS_ZOMBIES_TIMEER_H
