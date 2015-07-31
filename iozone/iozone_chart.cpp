#include "iozone/iozone_chart.h"

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

    //this->setBackground(QBrush(QColor(30, 30, 30)));
    this->xAxis->setBasePen(QPen(Qt::white, 1));
    this->yAxis->setBasePen(QPen(Qt::white, 1));
    this->xAxis->setTickPen(QPen(Qt::white, 1));
    this->yAxis->setTickPen(QPen(Qt::white, 1));
    this->xAxis->setSubTickPen(QPen(Qt::white, 1));
    this->yAxis->setSubTickPen(QPen(Qt::white, 1));
    this->xAxis->setTickLabelColor(Qt::white);
    this->yAxis->setTickLabelColor(Qt::white);
    this->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    this->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    this->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    this->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    this->xAxis->grid()->setSubGridVisible(true);
    this->yAxis->grid()->setSubGridVisible(true);
    this->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    this->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    this->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    this->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    this->xAxis->setLabelColor(QColor(200, 200, 200));
    this->yAxis->setLabelColor(QColor(200, 200, 200));
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, this->geometry().height());
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, this->geometry().height());
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    this->axisRect()->setBackground(axisRectGradient);
    this->setBackground(plotGradient);

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
    this->xAxis->setRange(0, 14);
    this->xAxis->setLabel("Reclen (kB)");
    this->xAxis->setPadding(5);

    // prepare y axis:
    /* y axis的范围取决于ratetype(column) 缺省给出最小范围 */
    this->yAxis->setRange(0, MINIMUM_DATA_RANGE);
    this->yAxis->setPadding(5); // a bit more space to the left border
    this->yAxis->setLabel( qslRateName[iRateType] + " Rate (kBytes/sec)"); /* y axis名由rateType定 */

    // setup legend:
    this->legend->setVisible(false);
    this->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    this->legend->setBrush(QColor(80, 80, 80, 200));
    QPen legendPen;
    legendPen.setColor(QColor(200, 200, 200, 200));
    this->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    this->legend->setTextColor(QColor(200, 200, 200, 200));
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
