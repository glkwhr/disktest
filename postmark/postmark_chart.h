#ifndef POSTMARK_CHART
#define POSTMARK_CHART

#include "qcustomplot/qcustomplot.h"
#include "postmark/postmark_thread.h"

class postmarkChart : public QCustomPlot
{
    Q_OBJECT
public:
    QTableWidget *table;
    QCPBarsGroup *bargroup;
    int kind;   //0 create append read delete
                //1 read data write data

    QCPBars *bars[FS_NUM];


public:
    postmarkChart(int k, QTableWidget *table, QWidget *parent = 0);
    void update();

signals:

public slots:

};

#endif // POSTMARK_CHART

