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
    openAction = new QAction(QIcon(":/images/file-open.png"), tr("&Open"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));

    saveAction = new QAction(QIcon(":/images/file-save.png"), tr("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save a new file"));

    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(openAction);
    file->addAction(saveAction);

    QToolBar *toolBar = addToolBar(tr("&File"));
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);

    QTabWidget * tabwidget = new QTabWidget(this);
    setCentralWidget(tabwidget);

    widget1 = new iozoneWidget(this);
    widget2 = new postmarkWidget(this);

    QIcon icon1(":/icons/iozone_icon.png");
    tabwidget->addTab(widget1, icon1, "Iozone");

    QIcon icon2(":/icons/postmark.png");
    tabwidget->addTab(widget2, icon2, "Postmark");




    connect(openAction, SIGNAL(QAction::triggered()), this, SLOT(MainWindow::openfile()));
    connect(saveAction, SIGNAL(QAction::triggered()), this, SLOT(MainWindow::savefile()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::openfile()
{



}
void MainWindow::savefile()
{

}










bool MainWindow::event(QEvent *e)
{
    if (e->type() == iozoneEvent::iozoneEventType)
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
