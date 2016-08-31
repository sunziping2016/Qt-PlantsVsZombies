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
    template <typename Functor>
    Timer(QObject *parent, int timeout, Functor functor) : QTimer(parent)
    {
        setInterval(timeout);
        setSingleShot(true);
        connect(this, &Timer::timeout, functor);
    }
};

class TimeLine: public QTimeLine
{
public:
    template <typename Functor1, typename Functor2>
    TimeLine(QObject *parent, int duration, int interval, Functor1 onChanged, Functor2 onFinished = [] {})
            : QTimeLine(duration, parent)
    {
        setUpdateInterval(interval);
        connect(this, &TimeLine::valueChanged, onChanged);
        connect(this, &TimeLine::finished, onFinished);
    }
};

#endif //PLANTS_VS_ZOMBIES_TIMEER_H
