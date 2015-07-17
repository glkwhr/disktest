#include "mainwindow.h"
#include <QApplication>

#include "iozone_event.h"
#include "postmark_event.h"

#include <QCoreApplication>

MainWindow *g_w;


const QEvent::Type iozoneEvent::iozoneEventType = (QEvent::Type)QEvent::registerEventType(QEvent::User + 100);
const QEvent::Type postmarkEvent::postmarkEventType = (QEvent::Type)QEvent::registerEventType(QEvent::User + 101);





extern "C" {
    void notifyGUI(int type, long long kb, long long reclen, unsigned long long speed)
    {
        iozoneEvent *iozevent = new iozoneEvent(type, kb, reclen, speed);
        QCoreApplication::postEvent(g_w, iozevent);
    }

    void notifyGUI_2_int(int isprogress, int type, int total, int persecond)
    {
        postmarkEvent *postmarkevent = new postmarkEvent(isprogress, type, total, persecond);
        QCoreApplication::postEvent(g_w, postmarkevent);
    }
    void notifyGUI_2_float(int isprogress, int type, float total, float persecond)
    {
        postmarkEvent *postmarkevent = new postmarkEvent(isprogress, type, total, persecond);
        QCoreApplication::postEvent(g_w, postmarkevent);
    }
}




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    g_w = new MainWindow();
    g_w->show();

    return a.exec();
}