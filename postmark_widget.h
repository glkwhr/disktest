/*postmark测试的主面板
包括参数选择的界面，开始按钮，显示返回信息的文本框和进度条*/

#ifndef POSTMARK_WIDGET
#define POSTMARK_WIDGET

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>

#include "paintedwidget.h"

#include "postmark_paramwidget.h"


class postmarkWidget : public QWidget
{
    Q_OBJECT
public:
    void buttonclicked();

    void myEventHandle(QEvent * e);

    postmarkWidget(QWidget * parent);

private:
    QString *qstring_textEdit1;
    QPushButton *button;
    QTextEdit *textEdit1;

    
    PaintedWidget *paintwidget;



    postmarkParamWidget * paramWidget;

    QLabel *pgslabel;
    QProgressBar * pgsbar;
};

#endif // POSTMARK_WIDGET

