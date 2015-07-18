#ifndef IOZONE_THREAD
#define IOZONE_THREAD

#include <QThread>
#include <QObject>
#include <QProcess>
#include <QDateTime>

#include "iozone_paramwidget.h"

#define DEFAULT_ARGC 5
#define DEFAULT_ARGV 32
#define DEFAULT_ARG_CHAR 128

class iozoneThread : public QThread
{
    Q_OBJECT
public:
    struct iozoneParamStruct * param;
    iozoneThread(struct iozoneParamStruct * p, QObject * parent = 0) : QThread(parent), param(p)
    {

    }
public:
    void run();
};

#endif // IOZONE_THREAD

