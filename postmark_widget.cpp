#include "postmark_widget.h"
#include "postmark_paramwidget.h"
#include "paintedwidget.h"

#include "postmark_thread.h"

#include "postmark_event.h"

#include <QFileDialog>
#include <QTextStream>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>



postmarkWidget::postmarkWidget(QWidget * parent):QWidget(parent)
{
    button = new QPushButton(this);
    button->setText(tr("Begin postmark Test"));
    connect(button, &QPushButton::clicked, this, &postmarkWidget::buttonclicked);

    textEdit1 = new QTextEdit(this);
    paintwidget = new PaintedWidget(this);

    qstring_textEdit1 = new QString();

    paramWidget = new postmarkParamWidget(this);


    pgsbar = new QProgressBar(this);
    pgsbar->setVisible(false);
    pgslabel = new QLabel(this);
    pgslabel->setText("");

    QGridLayout * gridlayout = new QGridLayout;
    gridlayout->addWidget(button, 1, 0);
    gridlayout->addWidget(textEdit1, 0, 1);
    gridlayout->addWidget(paintwidget, 1, 1);
    gridlayout->addWidget(paramWidget, 0, 0);

    gridlayout->addWidget(pgslabel, 2, 0);
    gridlayout->addWidget(pgsbar, 2, 1);

    this->setLayout(gridlayout);
}

//按钮按下的槽函数
void postmarkWidget::buttonclicked()
{
    struct postmark_param_struct * p = paramWidget->getParamData();
    postmarkThread * postmarkthread = new postmarkThread(p);

    //显示进度条
    pgsbar->setVisible(true);

    //开始postmark测试线程
    postmarkthread->start();
}


#define KILOBYTE 1024
#define MEGABYTE (KILOBYTE*KILOBYTE)

/* converts float values to byte/kilobyte/megabyte strings */
char *scalef(float i, char *buffer)
{


   if (i/(float)MEGABYTE>1)
      sprintf(buffer,"%.2f megabytes",i/(float)MEGABYTE);
   else
      if (i/(float)KILOBYTE)
         sprintf(buffer,"%.2f kilobytes",i/(float)KILOBYTE);
      else
         sprintf(buffer,"%f bytes",i);

   return (buffer);
}


void postmarkWidget::myEventHandle(QEvent *e)
{
    static char test_type_table[32][32] = {
        "total time", "transactions time", "created", "created along", "create mixed", "read", "append", "delete", "delete along", "delete mixed", "data read", "data written"
    };

    postmarkEvent * pe = (postmarkEvent *)e;


    if (((postmarkEvent *)e)->isprogress)
    {   //进度
        // progress message
        switch (pe->isprogress)
        {
        case 1:
            pgslabel->setText("Creating files...");
            pgsbar->setValue(pe->type);
            break;
        case 2:
            pgslabel->setText("Performing transactions...");
            pgsbar->setValue(pe->type);
            break;
        case 3:
            pgslabel->setText("Deleting files...");
            pgsbar->setValue(pe->type);
            break;
        case 4:
            pgslabel->setText("");
            pgsbar->setVisible(false);
            break;

        }
    }
    else
    {   //数据
        //data message




        QString tmp;

        if (((postmarkEvent *)e)->type != 10 && ((postmarkEvent *)e)->type != 11)
        {
            tmp.sprintf("%s--> %d  ( %d per second)\n",test_type_table[((postmarkEvent *)e)->type],
                        ((postmarkEvent *)e)->total, ((postmarkEvent *)e)->persecond );
        }
        else
        {
            char buff1[32];
            char buff2[32];
            tmp.sprintf("%s--> %s ( %s per second)\n",test_type_table[((postmarkEvent *)e)->type],
                        scalef( ((postmarkEvent *)e)->totalf, buff1) , scalef( ((postmarkEvent *)e)->persecondf, buff2 ) );
        }


        qstring_textEdit1->append(tmp);
        textEdit1->setText(*qstring_textEdit1);

    }

    e->accept();
}



