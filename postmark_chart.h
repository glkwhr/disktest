#ifndef POSTMARK_CHART
#define POSTMARK_CHART

#include "qcustomplot.h"


class postmarkChart : public QCustomPlot
{
    Q_OBJECT
private:
    QTableWidget *table;
    QCPBarsGroup *bargroup;
    int kind;   //0 create append read delete
                //1 read data write data


public:
    postmarkChart(int k, QTableWidget *table, QWidget *parent = 0);
    void update();

signals:

public slots:

};

#endif // POSTMARK_CHART

