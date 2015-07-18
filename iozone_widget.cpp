#include "iozone_widget.h"
#include "iozone_paramwidget.h"

#include "iozone_thread.h"
#include "iozone_event.h"

#include <QDebug>


/* 包括三个启动按钮 负责线程的创建和传参 */
iozoneWidget::iozoneWidget(QWidget * parent):QWidget(parent)
{
    /* iozone log checkbox */
    chkFlagIozoneLog = new QCheckBox(this);
    chkFlagIozoneLog->setText("Log");
    chkFlagIozoneLog->setChecked(false);/* 默认log表隐藏 */
    /* iozone log table */
    tableIozoneLog = new QTableWidget(this);
    tableIozoneLog->setColumnCount(8);
    /* 设置表格各列名称 */
    QStringList header;
    header<<"kb"<<"reclen"<<"write"<<"rewrite"<<"read"<<"reread"<<"randread"<<"randwrite";
    tableIozoneLog->setHorizontalHeaderLabels(header);
    tableIozoneLog->setVisible(chkFlagIozoneLog->isChecked());
    connect(chkFlagIozoneLog, SIGNAL(toggled(bool)), tableIozoneLog, SLOT(setVisible(bool)));

    /* progress bar */
    progressBar = new QProgressBar();
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setVisible(false);/* 进度条初始不可见 */

    /* start buttons */
    btnStartRamfs = new QPushButton(this);
    btnStartRamfs->setText(tr("Start RAMFS"));
    connect(btnStartRamfs, SIGNAL(clicked()), this, SLOT(onStartRamfs()));

    btnStartObfs = new QPushButton(this);
    btnStartObfs->setText(tr("Start OBFS"));
    connect(btnStartObfs, SIGNAL(clicked()), this, SLOT(onStartObfs()));

    btnStartPmfs = new QPushButton(this);
    btnStartPmfs->setText(tr("Start PMFS"));
    connect(btnStartPmfs, SIGNAL(clicked()), this, SLOT(onStartPmfs()));

    paramWidget = new iozoneParamWidget(this);

    QGridLayout *gridlayoutUp = new QGridLayout;
    QGridLayout *gridlayoutMain = new QGridLayout;
    gridlayoutUp->addWidget(paramWidget, 0, 0);
    gridlayoutUp->addWidget(btnStartRamfs, 0, 1);
    gridlayoutUp->addWidget(btnStartObfs, 0, 2);
    gridlayoutUp->addWidget(btnStartPmfs, 0, 3);
    gridlayoutMain->addLayout(gridlayoutUp, 0, 0);
    QSpacerItem *vSpacer0 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridlayoutMain->addItem(vSpacer0, 1, 0);
    gridlayoutMain->addWidget(chkFlagIozoneLog, 2, 0);
    gridlayoutMain->addWidget(tableIozoneLog, 3, 0);
    gridlayoutMain->addWidget(progressBar, 4, 0);

    this->setLayout(gridlayoutMain);
}


void iozoneWidget::onStartRamfs()
{
    if(paramWidget->getRunFlag()){
        /* 若正在进行测试则不作反应 */
        //qDebug()<<"running"<<endl;
        return;
    }
    paramWidget->setRunFlag(true);/* 测试开始 */
    /* 进度条置零并显示 */
    progressBar->setValue(0);
    progressBar->setVisible(true);

    /* 清空表格 */
    tableIozoneLog->clearContents();
    tableIozoneLog->setRowCount(0);
    struct iozoneParamStruct * p = paramWidget->getParamData(FILESYS_TYPE_RAMFS);
    iozoneThread * iozonethread = new iozoneThread(p);
    iozonethread->start();
}

void iozoneWidget::onStartObfs()
{
    //struct iozoneParamStruct * p = paramWidget->getParamData();
    //iozoneThread * iozonethread = new iozoneThread(p);
    //iozonethread->start();
}

void iozoneWidget::onStartPmfs()
{
    //struct iozoneParamStruct * p = paramWidget->getParamData();
    //iozoneThread * iozonethread = new iozoneThread(p);
    //iozonethread->start();
}

void iozoneWidget::myEventHandle(QEvent *e)
{
    ioZoneEvent *ioze = (ioZoneEvent *)e;
    const int iRowCount = tableIozoneLog->rowCount(); //获取表单行数
    static bool bRowInserted = false; //插入新行的标记
    static int iWriteRow = 0;
    static int iReadRow = 0;
    static int iRandRow = 0;
    QTableWidgetItem *item = new QTableWidgetItem();

    if(bRowInserted){
        /*插入了新行*/
        QTableWidgetItem *item0 = new QTableWidgetItem();
        QTableWidgetItem *item1 = new QTableWidgetItem();
        item0->setText( QString::number( ioze->llSize ) );
        item1->setText( QString::number( ioze->llRecLen ) );
        item0->setTextAlignment(Qt::AlignCenter);
        item1->setTextAlignment(Qt::AlignCenter);
        tableIozoneLog->setItem(iRowCount-1, 0, item0);
        tableIozoneLog->setItem(iRowCount-1, 1, item1);
        bRowInserted = false;

        /*改变进度条值*/
        progressBar->setValue(iRowCount*100/paramWidget->getTotalTimes());
    }

    switch( ioze->iType ){

    /*write_perf_test output*/
    case OUTPUT_TYPE_WRITE: /*write栏*/
        /*判断是否需要插入新行*/
        if( iWriteRow + 1 > iRowCount ) {
            tableIozoneLog->insertRow(iRowCount);
            bRowInserted = true;
        }
        /*创建表项并插入*/
        item->setText( QString::number( (long)(ioze->ullRate) ) );
        item->setTextAlignment(Qt::AlignCenter);
        tableIozoneLog->setItem(iWriteRow, 2+OUTPUT_TYPE_WRITE, item);
        break;

    case OUTPUT_TYPE_REWRITE: /*rewrite栏*/
        /*创建表项并插入*/
        item->setText( QString::number( ioze->ullRate ) );
        item->setTextAlignment(Qt::AlignCenter);
        tableIozoneLog->setItem(iWriteRow, 2+OUTPUT_TYPE_REWRITE, item);
         /*"光标"下移*/
        ++iWriteRow;
        if(progressBar->value() == 100)
        {   /*进度条已满*/
            iWriteRow = 0;
        }
        break;

    /*read_perf_test output*/
    case OUTPUT_TYPE_READ: /*read栏*/
        if( iReadRow + 1 > iRowCount ) {
            tableIozoneLog->insertRow(iRowCount);
            bRowInserted = true;
        }
        /*创建表项并插入*/
        item->setText( QString::number( (long)(ioze->ullRate) ) );
        item->setTextAlignment(Qt::AlignCenter);
        tableIozoneLog->setItem(iReadRow, 2+OUTPUT_TYPE_READ, item);
        break;

    case OUTPUT_TYPE_REREAD: /*reread栏*/
        /*创建表项并插入*/
        item->setText( QString::number( ioze->ullRate ) );
        item->setTextAlignment(Qt::AlignCenter);
        tableIozoneLog->setItem(iReadRow, 2+OUTPUT_TYPE_REREAD, item);
        ++iReadRow;
        if(progressBar->value() == 100)
        {   /*进度条已满*/
            iReadRow = 0;
        }
        break;

    /*random_perf_test output*/
    case OUTPUT_TYPE_RANDREAD: /*randread栏*/
        if( iRandRow + 1 > iRowCount ) {
            tableIozoneLog->insertRow(iRowCount);
            bRowInserted = true;
        }
        /*创建表项并插入*/
        item->setText( QString::number( (long)(ioze->ullRate) ) );
        item->setTextAlignment(Qt::AlignCenter);
        tableIozoneLog->setItem(iRandRow, 2+OUTPUT_TYPE_RANDREAD, item);
        break;

    case OUTPUT_TYPE_RANDWRITE: /*randwrite栏*/
        /*创建表项并插入*/
        item->setText( QString::number( ioze->ullRate ) );
        item->setTextAlignment(Qt::AlignCenter);
        tableIozoneLog->setItem(iRandRow, 2+OUTPUT_TYPE_RANDWRITE, item);
        ++iRandRow;
        if(progressBar->value() == 100)
        {   /*进度条已满*/
            iRandRow = 0;
        }
        break;
    }

    if(progressBar->value() == 100)
    {   /*进度条已满*/
        progressBar->setVisible(false);
    }
    e->accept();
}


