#include "iozone_event.h"

ioZoneEvent::ioZoneEvent():QEvent(ioZoneEvent::ioZoneEventType)
{
}

ioZoneEvent::ioZoneEvent(int it, long long lls, long long llr, unsigned long long ullrate ):
    QEvent(ioZoneEvent::ioZoneEventType)
{
    iType = it;
    llSize = lls;
    llRecLen = llr;
    ullRate = ullrate;
}
