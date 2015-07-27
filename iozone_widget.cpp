#include "iozone_widget.h"

#include <QDebug>


/* 包括三个启动按钮 负责线程的创建和传参 */
iozoneWidget::iozoneWidget(QWidget * parent):QWidget(parent)
{
    iCurFsType = 0;

    /* progress bar */
    labelStatus = new QLabel();
    labelStatus->setVisible(false);
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

    /* chart view combo box */
    QStringList qslRateType;
    qslRateType<< "Write" << "Rewrite" << "Read" << "Reread" << "RandRead" << "RandWrite";
    cboRateType = new QComboBox();
    cboRateType->addItems(qslRateType);
    /* chart view */
    QFrame *chartFrame = new QFrame(this);/* 用于设置chart边框 */
    chartFrame->setFrameStyle(QFrame::Panel);
    chartFrame->setFrameShadow(QFrame::Sunken);
    chartView = new iozoneChart(chartFrame);
    rateModel = new iozoneRateModel(this);
    chartView->setModel(0, rateModel);/* 设置chartview的显示类型 */
    connect(cboRateType, SIGNAL(currentIndexChanged(int)), chartView, SLOT(setRateType(const int)));

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

    QGridLayout *gridlayoutUp = new QGridLayout;
    QGridLayout *gridlayoutMain = new QGridLayout;
    gridlayoutUp->addWidget(paramWidget, 0, 0);
    gridlayoutUp->addWidget(btnStartRamfs, 0, 1);
    gridlayoutUp->addWidget(btnStartObfs, 0, 2);
    gridlayoutUp->addWidget(btnStartPmfs, 0, 3);
    gridlayoutMain->addLayout(gridlayoutUp, 0, 0, 1, 2);
    //QSpacerItem *vSpacer0 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridlayoutMain->addWidget(cboRateType, 1, 0, 1, 2);
    gridlayoutMain->addWidget(chartFrame, 2, 0, 1, 2);
    gridlayoutMain->addWidget(chkFlagIozoneLog, 3, 0, 1, 2);
    gridlayoutMain->addWidget(tableIozoneLog, 4, 0, 1, 2);
    gridlayoutMain->addWidget(labelStatus, 5, 0);
    gridlayoutMain->addWidget(progressBar, 5, 1);

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
    //progressBar->setRange(0, 100*(paramWidget->getTestTimes()) );/* 进度条范围 */
    progressBar->setVisible(true);
    labelStatus->setVisible(true);
    /* 设置当前测试文件系统类型 */
    iCurFsType = FILESYS_TYPE_RAMFS;
    /* 清空表格 */
    tableIozoneLog->clearContents();
    tableIozoneLog->setRowCount(0);
    struct iozoneParamStruct * p = paramWidget->getParamData(FILESYS_TYPE_RAMFS);
    iozoneThread *iozonethread = new iozoneThread(p, this);
    //connect(iozonethread, SIGNAL(finished()), chartView, SLOT(update()));
    connect(iozonethread, SIGNAL(finished()), iozonethread, SLOT(deleteLater()));
    iozonethread->start();
}

void iozoneWidget::onStartObfs()
{
    if(paramWidget->getRunFlag()){
        /* 若正在进行测试则不作反应 */
        //qDebug()<<"running"<<endl;
        return;
    }
    paramWidget->setRunFlag(true);/* 测试开始 */
    /* 进度条置零并显示 */
    progressBar->setValue(0);
    //progressBar->setRange(0, 100*(paramWidget->getTestTimes()) );/* 进度条范围 */
    progressBar->setVisible(true);
    labelStatus->setVisible(true);
    /* 设置当前测试文件系统类型 */
    iCurFsType = FILESYS_TYPE_OBFS;
    /* 清空表格 */
    tableIozoneLog->clearContents();
    tableIozoneLog->setRowCount(0);
    struct iozoneParamStruct * p = paramWidget->getParamData(FILESYS_TYPE_OBFS);
    iozoneThread *iozonethread = new iozoneThread(p, this);
    connect(iozonethread, SIGNAL(finished()), iozonethread, SLOT(deleteLater()));
    iozonethread->start();
}

void iozoneWidget::onStartPmfs()
{
    if(paramWidget->getRunFlag()){
        /* 若正在进行测试则不作反应 */
        //qDebug()<<"running"<<endl;
        return;
    }
    paramWidget->setRunFlag(true);/* 测试开始 */
    /* 进度条置零并显示 */
    progressBar->setValue(0);
    //progressBar->setRange(0, 100*(paramWidget->getTestTimes()) );/* 进度条范围 */
    progressBar->setVisible(true);
    labelStatus->setVisible(true);
    /* 设置当前测试文件系统类型 */
    iCurFsType = FILESYS_TYPE_PMFS;
    /* 清空表格 */
    tableIozoneLog->clearContents();
    tableIozoneLog->setRowCount(0);
    struct iozoneParamStruct * p = paramWidget->getParamData(FILESYS_TYPE_PMFS);
    iozoneThread *iozonethread = new iozoneThread(p, this);
    connect(iozonethread, SIGNAL(finished()), iozonethread, SLOT(deleteLater()));
    iozonethread->start();
}

void iozoneWidget::myEventHandle(QEvent *e)
{
    ioZoneEvent *ioze = (ioZoneEvent *)e;
    const int iRowCount = tableIozoneLog->rowCount(); //获取表单行数
    static bool sbRowInserted = false; //插入新行的标记
    /* 以下三个用于在一次iozone测试中记录"光标"位置 */
    static int siWriteRow = 0;
    static int siReadRow = 0;
    static int siRandRow = 0;
    static int siCurTimes = 0; /* 当前正进行第几次测试 */
    QTableWidgetItem *item = new QTableWidgetItem();

    /* log table 中的数据显示每次测试的结果 不进行平均处理 同时作为单次进度条的依据 */
    if(sbRowInserted){
        /*插入了新行*/
        QTableWidgetItem *item0 = new QTableWidgetItem();
        QTableWidgetItem *item1 = new QTableWidgetItem();
        item0->setText( QString::number( ioze->llSize ) );
        item1->setText( QString::number( ioze->llRecLen ) );
        item0->setTextAlignment(Qt::AlignCenter);
        item1->setTextAlignment(Qt::AlignCenter);
        tableIozoneLog->setItem(iRowCount-1, 0, item0);
        tableIozoneLog->setItem(iRowCount-1, 1, item1);
        sbRowInserted = false;

        /*改变进度条值*/
        progressBar->setValue(iRowCount*100/paramWidget->getTotalTimes());
    }
    labelStatus->setText( QString("%1/%2").arg(siCurTimes+1).arg(paramWidget->getTestTimes() ) );
    unsigned long long tmpRate;
    switch( ioze->iType ){

    /*write_perf_test output*/
    case OUTPUT_TYPE_WRITE: /*write栏*/
        /*判断是否需要插入新行*/
        if( siWriteRow + 1 > iRowCount ) {
            tableIozoneLog->insertRow(iRowCount);
            sbRowInserted = true;
        }
        /*创建表项并插入*/
        item->setText( QString::number( (long)(ioze->ullRate) ) );
        item->setTextAlignment(Qt::AlignCenter);
        tableIozoneLog->setItem(siWriteRow, 2+OUTPUT_TYPE_WRITE, item);
        /* 更新数据模型 */
        //unsigned long long tmpRate;
        tmpRate = ( rateModel->data(iCurFsType, siWriteRow, ioze->iType) * siCurTimes + ioze->ullRate ) / (siCurTimes + 1);
        rateModel->addRawRateData(iCurFsType, ioze->llRecLen, ioze->iType, tmpRate);
        break;

    case OUTPUT_TYPE_REWRITE: /*rewrite栏*/
        /*创建表项并插入*/
        item->setText( QString::number( ioze->ullRate ) );
        item->setTextAlignment(Qt::AlignCenter);
        tableIozoneLog->setItem(siWriteRow, 2+OUTPUT_TYPE_REWRITE, item);
        /* 更新数据模型 */
        //unsigned long long tmpRate;
        tmpRate = ( rateModel->data(iCurFsType, siWriteRow, ioze->iType) * siCurTimes + ioze->ullRate ) / (siCurTimes + 1);
        rateModel->addRawRateData(iCurFsType, ioze->llRecLen, ioze->iType, tmpRate);
         /*"光标"下移*/
        ++siWriteRow;
        if(progressBar->value() == 100)
        {   /*单次进度条已满*/
            siWriteRow = 0;
        }
        break;

    /*read_perf_test output*/
    case OUTPUT_TYPE_READ: /*read栏*/
        if( siReadRow + 1 > iRowCount ) {
            tableIozoneLog->insertRow(iRowCount);
            sbRowInserted = true;
        }
        /*创建表项并插入*/
        item->setText( QString::number( (long)(ioze->ullRate) ) );
        item->setTextAlignment(Qt::AlignCenter);
        tableIozoneLog->setItem(siReadRow, 2+OUTPUT_TYPE_READ, item);
        /* 更新数据模型 */
        //unsigned long long tmpRate;
        tmpRate = ( rateModel->data(iCurFsType, siReadRow, ioze->iType) * siCurTimes + ioze->ullRate ) / (siCurTimes + 1);
        rateModel->addRawRateData(iCurFsType, ioze->llRecLen, ioze->iType, tmpRate);
        break;

    case OUTPUT_TYPE_REREAD: /*reread栏*/
        /*创建表项并插入*/
        item->setText( QString::number( ioze->ullRate ) );
        item->setTextAlignment(Qt::AlignCenter);
        tableIozoneLog->setItem(siReadRow, 2+OUTPUT_TYPE_REREAD, item);
        /* 更新数据模型 */
        //unsigned long long tmpRate;
        tmpRate = ( rateModel->data(iCurFsType, siReadRow, ioze->iType) * siCurTimes + ioze->ullRate ) / (siCurTimes + 1);
        rateModel->addRawRateData(iCurFsType, ioze->llRecLen, ioze->iType, tmpRate);
        ++siReadRow;
        if(progressBar->value() == 100)
        {   /*单次进度条已满*/
            siReadRow = 0;
        }
        break;

    /*random_perf_test output*/
    case OUTPUT_TYPE_RANDREAD: /*randread栏*/
        if( siRandRow + 1 > iRowCount ) {
            tableIozoneLog->insertRow(iRowCount);
            sbRowInserted = true;
        }
        /*创建表项并插入*/
        item->setText( QString::number( (long)(ioze->ullRate) ) );
        item->setTextAlignment(Qt::AlignCenter);
        tableIozoneLog->setItem(siRandRow, 2+OUTPUT_TYPE_RANDREAD, item);
        /* 更新数据模型 */
        //unsigned long long tmpRate;
        tmpRate = ( rateModel->data(iCurFsType, siRandRow, ioze->iType) * siCurTimes + ioze->ullRate ) / (siCurTimes + 1);
        rateModel->addRawRateData(iCurFsType, ioze->llRecLen, ioze->iType, tmpRate);
        break;

    case OUTPUT_TYPE_RANDWRITE: /*randwrite栏*/
        /*创建表项并插入*/
        item->setText( QString::number( ioze->ullRate ) );
        item->setTextAlignment(Qt::AlignCenter);
        tableIozoneLog->setItem(siRandRow, 2+OUTPUT_TYPE_RANDWRITE, item);
        /* 更新数据模型 */
        //unsigned long long tmpRate;
        tmpRate = ( rateModel->data(iCurFsType, siRandRow, ioze->iType) * siCurTimes + ioze->ullRate ) / (siCurTimes + 1);
        rateModel->addRawRateData(iCurFsType, ioze->llRecLen, ioze->iType, tmpRate);
        ++siRandRow;
        if(progressBar->value() == 100)
        {
            siRandRow = 0;
            /*单次进度条已满 由于randwrite一定是一次测试中最后的输出
            故将结束时的处理放在此处 但前提是进行测试的项目包括randr/w
            即参数需包括 "-i 2" */
            progressBar->setValue(0);
            ++siCurTimes;
            chartView->update();
            if ( siCurTimes == paramWidget->getTestTimes() )
            {   /* 多轮测试都已结束 */
                siCurTimes = 0;
                progressBar->setVisible(false);
                labelStatus->setText("");
                labelStatus->setVisible(false);
            }
            else
            {
                /* 清空表格 */
                tableIozoneLog->clearContents();
                tableIozoneLog->setRowCount(0);
            }
        }
        break;
    }
    e->accept();
}


