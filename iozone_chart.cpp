#include "iozone_chart.h"

iozoneChart::iozoneChart(QWidget *parent) :
    QCustomPlot(parent)
{
    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks<<1 << 2 << 3;
    labels << "4"<<"8"<<"16";
    this->xAxis->setAutoTicks(false);
    this->xAxis->setAutoTickLabels(false);
    this->xAxis->setTickVector(ticks);
    this->xAxis->setTickVectorLabels(labels);
    this->xAxis->setTickLabelRotation(60);
    this->xAxis->setSubTickCount(0);
    this->xAxis->setTickLength(0, 4);
    this->xAxis->grid()->setVisible(true);
    this->xAxis->setRange(0, 8);

    /* add graphs with different line styles */
    QCPGraph *ramfs = new QCPGraph(this->xAxis, this->yAxis);/* 加一条折线 */
    this->addPlottable(ramfs);
    /* set names and colors */
    QPen pen;
    pen.setWidthF(1.2);/* 轮廓宽度 */
    ramfs->setName("RAMFS");
    pen.setColor(QColor(150, 222, 0));
    ramfs->setPen(pen);
    ramfs->setBrush(QColor(150, 222, 0, 70));
    ramfs->setLineStyle(QCPGraph::lsLine);
    ramfs->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    /* set data */
    QVector<double> ramfsData;
    ramfsData<<8<<9<<7;
    ramfs->setData(ticks, ramfsData);
    ramfs->rescaleAxes(true);

    // prepare y axis:
    this->yAxis->setRange(0, 12.1);
    this->yAxis->setPadding(5); // a bit more space to the left border
    this->yAxis->setLabel("Power Consumption in\nKilowatts per Capita (2007)");
    this->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    this->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    this->yAxis->grid()->setSubGridPen(gridPen);

    // setup legend:
    this->legend->setVisible(true);
    this->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    this->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    this->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    this->legend->setFont(legendFont);
}
