#ifndef POSTMARK_EVENT
#define POSTMARK_EVENT

#include <QEvent>

class postmarkEvent : public QEvent
{
public:
    static const QEvent::Type postmarkEventType;
    postmarkEvent(int isprogress, int type, int total, int persecond);
    postmarkEvent(int isprogress, int type, float totalf, float persecondf);

    int isprogress; //0 not progress  1 creating 2 transactioning 3 deleting
    int type;   /*
                    0  seconds total
                    1 seconds of transactions

                    2 created
                    3 create along
                    4 create mixed
                    5 read
                    6 append
                    7 delete
                    8 delete along
                    9 delete mixed

                    10 data read
                    11 data written
                */


    int total;   //how many
    int persecond;  //per second

    float totalf;
    float persecondf;

};

#endif // POSTMARK_EVENT

