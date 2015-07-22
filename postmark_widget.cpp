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

#include <QStringList>


char global_fschararray[FS_NUM][16] = {"ramfs", "tmpfs","ext2", "ext3", "ext4"};


postmarkWidget::postmarkWidget(QWidget * parent):QWidget(parent)
{
    button = new QPushButton(this);
    button->setText(tr("Begin postmark Test"));
    connect(button, SIGNAL(clicked()), this, SLOT(buttonclicked()));

    table = new QTableWidget(FS_NUM, 6, this);

    QStringList header;
    header << "Create" << "Read" << "Appended" << "Delete" << "Read(data)" << "Write data";
    table->setHorizontalHeaderLabels(header);





    //paintwidget = new PaintedWidget(this);

    parambutton = new QPushButton(this);
    parambutton->setText("<<< Details setting");
    connect(parambutton, SIGNAL(clicked()), this, SLOT(parambuttonclicked()));

    paramWidget = new postmarkParamWidget(this);
    paramWidget->setVisible(false);


    QHBoxLayout *selectfslayout = new QHBoxLayout;
    for (int i = 0; i < FS_NUM; ++i)
    {
        selectfs[i] = new QCheckBox(global_fschararray[i], this);
        selectfs[i]->setChecked(true);
        selectfslayout->addWidget(selectfs[i]);
    }



    fslabel = new QLabel(this);
    fslabel->setText("");

    pgsbar = new QProgressBar(this);
    pgsbar->setVisible(false);
    pgslabel = new QLabel(this);
    pgslabel->setText("");

    QGridLayout * gridlayout = new QGridLayout;
    gridlayout->addWidget(button, 1, 1);
    gridlayout->addWidget(table, 3, 1, 1, 2);
    //gridlayout->addWidget(paintwidget, 1, 1);


    gridlayout->addWidget(parambutton, 0, 1);
    gridlayout->addWidget(paramWidget, 0, 0, 5, 1);

    gridlayout->addLayout(selectfslayout, 0, 2);


    gridlayout->addWidget(fslabel, 4, 1, 1, 2);
    gridlayout->addWidget(pgslabel, 5, 1);
    gridlayout->addWidget(pgsbar, 5, 2);

    this->setLayout(gridlayout);
}

//按钮按下的槽函数
void postmarkWidget::buttonclicked()
{
    table->setRowCount(0);
    table->setRowCount(FS_NUM);
    QStringList header;
    header.clear();
    for (int i = 0; i < FS_NUM; ++i)
    {
        header << global_fschararray[i];
    }
    table->setVerticalHeaderLabels(header);

    struct postmark_param_struct * p = paramWidget->getParamData();

    int whichfs = 0;
    for (int i = 0; i < FS_NUM; ++i)
    {
        if (selectfs[i]->isChecked())
        {
            whichfs |= (1 << i);
        }
    }



    postmarkThread * postmarkthread = new postmarkThread(pgsbar, whichfs, p, this->fslabel);

    //显示进度条


    //开始postmark测试线程
    postmarkthread->start();

}
void postmarkWidget::parambuttonclicked()
{
    paramWidget->setVisible(!paramWidget->isVisible());
    if (paramWidget->isVisible())
        parambutton->setText(">>> Details setting");
    else
        parambutton->setText("<<< Details setting");
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



int eventidTotableindex(int num)
{
    //2  5 6 7 10 11
    switch (num)
    {
    case 2:
        return 0;
        break;
    case 5:
        return 1;
        break;
    case 6:
        return 2;
        break;
    case 7:
        return 3;
        break;
    case 10:
        return 4;
        break;
    case 11:
        return 5;
        break;
    default:
        return -1;

    }
}

void postmarkWidget::myEventHandle(QEvent *e)
{
   /* static char test_type_table[32][32] = {
        "total time", "transactions time", "created", "created along", "create mixed", "read", "append", "delete", "delete along", "delete mixed", "data read", "data written"
    };*/

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


        }
    }
    else
    {   //数据
        //data message
        if (eventidTotableindex(pe->type) != -1)
        {
            if (((postmarkEvent *)e)->type != 10 && ((postmarkEvent *)e)->type != 11)
            {
                table->setItem(pe->nowfs, eventidTotableindex(pe->type), new QTableWidgetItem(QString::number(pe->persecond)));
            }
            else
            {
                char buff[64];
                table->setItem(pe->nowfs, eventidTotableindex(pe->type), new QTableWidgetItem(scalef(pe->persecondf, buff)));
            }
        }



    }

    e->accept();
}



