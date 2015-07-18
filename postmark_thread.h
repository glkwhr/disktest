#ifndef POSTMARK_THREAD
#define POSTMARK_THREAD

#include <QThread>
#include <QObject>

#include <QLabel>

class postmarkThread : public QThread
{
    Q_OBJECT
public:

    struct postmark_param_struct * param;
    QLabel *label;

    postmarkThread(struct postmark_param_struct * p, QLabel *l, QObject * parent = 0) : QThread(parent), param(p), label(l)
    {

    }
public:
    void run();
};

#endif // POSTMARK_THREAD

