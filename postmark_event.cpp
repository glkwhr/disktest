#include "postmark_event.h"

postmarkEvent::postmarkEvent(int isp, int t, int to, int per) :
    QEvent(postmarkEvent::postmarkEventType), isprogress(isp), type(t), total(to), persecond(per)
{

}

postmarkEvent::postmarkEvent(int isp, int t, float to, float per) :
    QEvent(postmarkEvent::postmarkEventType), isprogress(isp), type(t), totalf(to), persecondf(per)
{

}
