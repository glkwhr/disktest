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

#include "postmark/postmark_thread.h"

#include "postmark/postmark_paramwidget.h"

#include "postmark/postmark_chart.h"

class postmarkWidget : public QWidget
{
    Q_OBJECT
public slots:
    void buttonclicked();
    void parambuttonclicked();
    void fsbuttonclicked();

public:
    void myEventHandle(QEvent * e);

    postmarkWidget(QWidget * parent);


private:
    QCheckBox * selectfs[FS_NUM];



    QPushButton *button;

    QCheckBox *showtable;
    QTableWidget *table;

    postmarkChart *chart1;
    postmarkChart *chart2;



    QPushButton *parambutton;
    postmarkParamWidget * paramWidget;

    QLabel * fslabel;

    QLabel *pgslabel;
    QProgressBar * pgsbar;

    QPushButton *fsbutton;
    QWidget * selectfswidget;
};

#endif // POSTMARK_WIDGET

