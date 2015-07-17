#include "iozone_widget.h"
#include "iozone_paramwidget.h"
#include "paintedwidget.h"

#include "iozone_thread.h"

#include "iozone_event.h"

#include <QFileDialog>
#include <QTextStream>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>



iozoneWidget::iozoneWidget(QWidget * parent):QWidget(parent)
{
    button = new QPushButton(this);
    button->setText(tr("Begin Iozone Test"));
    connect(button, &QPushButton::clicked, this, &iozoneWidget::buttonclicked);

    textEdit1 = new QTextEdit(this);
    paintwidget = new PaintedWidget(this);

    qstring_textEdit1 = new QString();

    paramWidget = new iozoneParamWidget(this);

    QGridLayout * gridlayout = new QGridLayout;
    gridlayout->addWidget(button, 1, 0);
    gridlayout->addWidget(textEdit1, 0, 1);
    gridlayout->addWidget(paintwidget, 1, 1);
    gridlayout->addWidget(paramWidget, 0, 0);

    this->setLayout(gridlayout);
}


void iozoneWidget::buttonclicked()
{
    struct iozone_param_struct * p = paramWidget->getParamData();
    iozoneThread * iozonethread = new iozoneThread(p);
    iozonethread->start();
}


void iozoneWidget::myEventHandle(QEvent *e)
{
    static char test_type_table[32][32] = {
        "write", "re-write", "read", "re-read","random read", "random write"
    };


    QString tmp;
    tmp.sprintf("%s--> kb: %lld    reclen: %lld   speed: %lld\n",test_type_table[((iozoneEvent *)e)->type],
                ((iozoneEvent *)e)->kb, ((iozoneEvent *)e)->reclen, ((iozoneEvent *)e)->speed);

    qstring_textEdit1->append(tmp);
    textEdit1->setText(*qstring_textEdit1);
    e->accept();
}


