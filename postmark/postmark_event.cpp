#include "postmark/postmark_event.h"

postmarkEvent::postmarkEvent(int now, int isp, int t, int to, int per) :
    QEvent(postmarkEvent::postmarkEventType), nowfs(now), isprogress(isp), type(t), total(to), persecond(per)
{

}

postmarkEvent::postmarkEvent(int now, int isp, int t, float to, float per) :
    QEvent(postmarkEvent::postmarkEventType), nowfs(now), isprogress(isp), type(t), totalf(to), persecondf(per)
{

}
