#include "mainwindow.h"

#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>



#include "iozone_event.h"


#include "iozone_widget.h"


#include "postmark_widget.h"
#include "postmark_event.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QTabWidget * tabwidget = new QTabWidget(this);
    setCentralWidget(tabwidget);

    widget1 = new iozoneWidget(this);
    widget2 = new postmarkWidget(this);

    QIcon icon1(":/icons/iozone_icon.png");
    tabwidget->addTab(widget1, icon1, "Iozone");

    QIcon icon2(":/icons/postmark.png");
    tabwidget->addTab(widget2, icon2, "Postmark");
}

MainWindow::~MainWindow()
{

}


bool MainWindow::event(QEvent *e)
{
    if (e->type() == ioZoneEvent::ioZoneEventType)
    {
        widget1->myEventHandle(e);
        if (e->isAccepted())
        {
            return true;
        }
    }
    else if (e->type() == postmarkEvent::postmarkEventType)
    {
        widget2->myEventHandle(e);
        if (e->isAccepted())
        {
            return true;
        }
    }
    else
    {
        return QMainWindow::event(e);
    }
}
