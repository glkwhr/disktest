#ifndef IOZONE_EVENT_H
#define IOZONE_EVENT_H

#include <QEvent>

class iozoneEvent : public QEvent
{
public:
    static const QEvent::Type iozoneEventType;
    iozoneEvent();
    iozoneEvent(int type, long long k, long long r, unsigned long long s);

    int type;   //0 write 1 rewrite 2 read 3 reread 4 random read 5 random write
    long long kb;   //file size
    long long reclen;   //record size
    unsigned long long speed;   //transmission speed
};

#endif // IOZONE_EVENT_H

