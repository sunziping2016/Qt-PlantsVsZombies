//
// Created by sun on 8/26/16.
//

#ifndef PLANTS_VS_ZOMBIES_TIMEOUT_H
#define PLANTS_VS_ZOMBIES_TIMEOUT_H


#include <QtCore>

class Timeout: public QTimer
{
public:
    Timeout(int interval, QObject *p = Q_NULLPTR, QObject *receiver = Q_NULLPTR, const char *member = Q_NULLPTR);
    template <typename Functor>
    Timeout(int interval, Functor functor, QObject *p = Q_NULLPTR)
            : QTimer(p)
    {
        setInterval(interval);
        setSingleShot(true);
        connect(this, &Timeout::timeout, functor);
    }

};


#endif //PLANTS_VS_ZOMBIES_TIMEOUT_H
