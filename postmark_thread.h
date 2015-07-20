#ifndef POSTMARK_THREAD
#define POSTMARK_THREAD

#include <QThread>
#include <QObject>

#include <QLabel>
#include <QProgressBar>

#define FS_NUM 5

class postmarkThread : public QThread
{
    Q_OBJECT
public:

    struct postmark_param_struct * param;
    QLabel *label;
    int whichfs;
    QProgressBar * pgs;

    postmarkThread(QProgressBar * pg, int w, struct postmark_param_struct * p, QLabel *l, QObject * parent = 0) : QThread(parent), pgs(pg), whichfs(w), param(p), label(l)
    {

    }
public:
    void run();
};

#endif // POSTMARK_THREAD

