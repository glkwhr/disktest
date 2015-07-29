#include "iozone_chart.h"

iozoneChart::iozoneChart(QFrame *frame, QWidget *parent) :
    QCustomPlot(parent)
{
    iRateType = 0;
    qslFsName << "RAMFS" << "OBFS" << "PMFS" << "EXT4";
    qslRateName << "Write" << "Rewrite" << "Read" << "Reread" << "RandRead" << "RandWrite";
    /* 应用frame */
    QGridLayout *gridlayout = new QGridLayout(frame);
    gridlayout->addWidget(this, 0, 0);
    gridlayout->setMargin(0);
    // prepare x axis with labels:
    for ( int i=1; i<=13; ++i ){
        ticks << i;
    }
    labels << "4"<<"8"<<"16"<<"32"<<"64"<<"128"<<"256"<<"512"<<"1024"<<"2048"<<"4096"<<"8192"<<"16384";
    this->xAxis->setAutoTicks(false);
    this->xAxis->setAutoTickLabels(false);
    this->xAxis->setTickVector(ticks);
    this->xAxis->setTickVectorLabels(labels);
    this->xAxis->setTickLabelRotation(30);
    this->xAxis->setSubTickCount(0);
    this->xAxis->setTickLength(0, 4);
    this->xAxis->grid()->setVisible(true);
    this->xAxis->setRange(0, 14);
    this->xAxis->setLabel("Reclen (kB)");
    this->xAxis->setPadding(5);

    // prepare y axis:
    /* y axis的范围取决于ratetype(column) 缺省给出最小范围 */
    this->yAxis->setRange(0, MINIMUM_DATA_RANGE);
    this->yAxis->setPadding(5); // a bit more space to the left border
    this->yAxis->setLabel( qslRateName[iRateType] + " Rate (kBytes/sec)"); /* y axis名由rateType定 */
    this->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    this->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    this->yAxis->grid()->setSubGridPen(gridPen);

    // setup legend:
    this->legend->setVisible(false);
    this->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    this->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    this->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    this->legend->setFont(legendFont);
}

void iozoneChart::setRateType(const int rateType)
{
    iRateType = rateType;
    this->update();
}

void iozoneChart::setModel(int rateType, iozoneRateModel *model)
{
    iRateType = rateType;/* 设定该chart显示的数据类型 */
    this->yAxis->setLabel( qslRateName[iRateType] + " Rate (kBytes/sec)");
    dataModel = model;
}

void iozoneChart::update(void)
{
    this->clearGraphs();
    this->legend->setVisible(true);
    this->yAxis->setRange(0, MINIMUM_DATA_RANGE);
    this->yAxis->setLabel( qslRateName[iRateType] + " Rate (kBytes/sec)");

    QVector<QCPGraph * > qvGraph;
    int i, j;
    i = j = 0;
    for ( i = FILESYS_TYPE_RAMFS; i < FILESYS_COUNT; ++i )
    {
        /* add graphs with different line styles */
        qvGraph << new QCPGraph(this->xAxis, this->yAxis);
        this->addPlottable(qvGraph[i]);
        /* set names and colors */
        QPen pen;
        pen.setWidthF(3);/* 轮廓宽度 */
        qvGraph[i]->setName( qslFsName[i] );
        switch ( i )
        {   /* 设置折线颜色和阴影颜色 */
        case FILESYS_TYPE_RAMFS:
            pen.setColor(QColor(150, 222, 0));
            qvGraph[i]->setBrush(QColor(150, 222, 0, 20));
            break;
        case FILESYS_TYPE_OBFS:
            pen.setColor(QColor(222, 150, 0));
            qvGraph[i]->setBrush(QColor(222, 150, 0, 20));
            break;
        case FILESYS_TYPE_PMFS:
            pen.setColor(QColor(0, 150, 222));
            qvGraph[i]->setBrush(QColor(0, 150, 222, 20));
            break;
        case FILESYS_TYPE_EXT4:
            pen.setColor(QColor(250, 111, 111));
            qvGraph[i]->setBrush(QColor(250, 111, 111, 20));
            break;
        }

        qvGraph[i]->setPen(pen);
        qvGraph[i]->setLineStyle(QCPGraph::lsLine);
        qvGraph[i]->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
        if ( dataModel->data(i, 0, iRateType) != 0 )
        {   /* set data */
            QVector<double> tmpData;
            for ( j = 0; j < dataModel->rowCount(i); ++j )
            {
                if ( dataModel->data(i, j, iRateType) != 0 )
                {
                    tmpData << dataModel->data(i, j, iRateType);
                }
                else
                    break;
            }
            qvGraph[i]->setData(ticks, tmpData);
            qvGraph[i]->rescaleAxes(true);
        }
    }
    this->replot();
}
