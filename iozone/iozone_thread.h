#ifndef IOZONE_THREAD
#define IOZONE_THREAD

#include <QThread>
#include <QObject>
#include <QProcess>
#include <QDateTime>
#include <QSystemSemaphore>
//#include <QSharedMemory>

#include "iozone/iozone_paramwidget.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define DEFAULT_ARGC 5
#define DEFAULT_ARGV 32
#define DEFAULT_ARG_CHAR 128

struct shmNotify{
    int notifyFlag;
    int type;
    long long kb;
    long long reclen;
    unsigned long long speed;
};

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
    void threadNotifyGUI(iozoneThread *, int type, long long kb, long long reclen, unsigned long long speed);
    void callThread(int type, long long kb, long long reclen, unsigned long long speed);

private:
    QSystemSemaphore *callFlag;
    QSystemSemaphore *waitFlag;
    void *shm;
    int shmid;
    struct shmNotify *pShmNotify;
};

#endif // IOZONE_THREAD

