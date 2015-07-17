#ifndef IOZONE_THREAD
#define IOZONE_THREAD

#include <QThread>
#include <QObject>

class iozoneThread : public QThread
{
    Q_OBJECT
public:
    struct iozone_param_struct * param;
    iozoneThread(struct iozone_param_struct * p, QObject * parent = 0) : QThread(parent), param(p)
    {

    }
public:
    void run();
};

#endif // IOZONE_THREAD

