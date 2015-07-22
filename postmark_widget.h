/*postmark测试的主面板
包括参数选择的界面，开始按钮，显示返回信息的文本框和进度条*/

#ifndef POSTMARK_WIDGET
#define POSTMARK_WIDGET

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>

#include <QTableWidget>

#include "paintedwidget.h"

#include "postmark_thread.h"

#include "postmark_paramwidget.h"

#include "qcustomplot.h"

class postmarkWidget : public QWidget
{
    Q_OBJECT
public slots:
    void buttonclicked();
    void parambuttonclicked();

public:
    void myEventHandle(QEvent * e);
    void displayBarGraph();

    postmarkWidget(QWidget * parent);


private:
    QCheckBox * selectfs[FS_NUM];



    QPushButton *button;
    QTableWidget *table;

    QCustomPlot *customPlot;



    
    //PaintedWidget *paintwidget;


    QPushButton *parambutton;
    postmarkParamWidget * paramWidget;

    QLabel * fslabel;

    QLabel *pgslabel;
    QProgressBar * pgsbar;
};

#endif // POSTMARK_WIDGET

