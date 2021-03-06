#include "mainwindow.h"

#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>



#include "iozone/iozone_event.h"
#include "iozone/iozone_widget.h"

#include "postmark/postmark_widget.h"
#include "postmark/postmark_event.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QTabWidget * tabwidget = new QTabWidget(this);
    setCentralWidget(tabwidget);

    widget1 = new iozoneWidget(this);
    widget2 = new postmarkWidget(this);

    //QIcon icon1(":/icons/iozone_tab.png");
    tabwidget->addTab(widget1, "Iozone");

    //QIcon icon2(":/icons/postmark.png");
    tabwidget->addTab(widget2, "Postmark");

    this->resize(860, 680);
    this->setWindowTitle("DiskTest");
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
