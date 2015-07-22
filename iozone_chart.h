#ifndef IOZONE_CHART_H
#define IOZONE_CHART_H

#include "qcustomplot.h"

/* 看作chartview */
class iozoneChart : public QCustomPlot
{
    Q_OBJECT
public:
    explicit iozoneChart(QWidget *parent = 0);

signals:

public slots:

};

#endif // IOZONE_CHART_H
