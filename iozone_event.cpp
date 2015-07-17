#include "iozone_event.h"

iozoneEvent::iozoneEvent():QEvent(iozoneEvent::iozoneEventType)
{

}

iozoneEvent::iozoneEvent(int t, long long k, long long r, unsigned long long s):
    QEvent(iozoneEvent::iozoneEventType), type(t), kb(k), reclen(r), speed(s)
{

}
