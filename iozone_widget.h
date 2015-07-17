#ifndef IOZONE_WIDGET
#define IOZONE_WIDGET

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include "paintedwidget.h"

#include "iozone_paramwidget.h"


class iozoneWidget : public QWidget
{
    Q_OBJECT
public:
    void buttonclicked();

    void myEventHandle(QEvent * e);

    iozoneWidget(QWidget * parent);

private:
    QString *qstring_textEdit1;
    QPushButton *button;
    QTextEdit *textEdit1;
    PaintedWidget *paintwidget;

    iozoneParamWidget * paramWidget;
};

#endif // IOZONE_WIDGET

