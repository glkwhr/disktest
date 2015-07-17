#ifndef POSTMARK_THREAD
#define POSTMARK_THREAD

#include <QThread>
#include <QObject>

class postmarkThread : public QThread
{
    Q_OBJECT
public:
    struct postmark_param_struct * param;
    postmarkThread(struct postmark_param_struct * p, QObject * parent = 0) : QThread(parent), param(p)
    {

    }
public:
    void run();
};

#endif // POSTMARK_THREAD

