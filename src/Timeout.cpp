//
// Created by sun on 8/26/16.
//

#include "Timeout.h"

Timeout::Timeout(int interval, QObject *p, QObject *receiver, const char *member)
        : QTimer(p)
{
    setInterval(interval);
    setSingleShot(true);
    if (receiver)
        connect(this, SIGNAL(timeout()), receiver, member);
}