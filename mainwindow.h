#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>

#include <QEvent>

#include "iozone/iozone_widget.h"
#include "postmark/postmark_widget.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    iozoneWidget * widget1;
    postmarkWidget * widget2;

    bool event(QEvent *event);
};



#endif // MAINWINDOW_H
