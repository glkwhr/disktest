#ifndef IOZONEEVENT_H
#define IOZONEEVENT_H

#include <QEvent>

class ioZoneEvent : public QEvent
{
public:
    ioZoneEvent();
    ioZoneEvent(int, long long, long long, unsigned long long);

    static const QEvent::Type ioZoneEventType;
    int iType;
    long long llSize;
    long long llRecLen;
    unsigned long long ullRate;
};

#endif // IOZONEEVENT_H
