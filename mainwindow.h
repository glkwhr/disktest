#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>

#include <QEvent>

#include "iozone_widget.h"
#include "postmark_widget.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:



    QAction *openAction;
    QAction *saveAction;

    iozoneWidget * widget1;
    postmarkWidget * widget2;





    bool event(QEvent *event);
public slots:
    void openfile();
    void savefile();
};



#endif // MAINWINDOW_H
