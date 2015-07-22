#ifndef POSTMARK_CHART
#define POSTMARK_CHART

#include "qcustomplot.h"


class postmarkChart : public QCustomPlot
{
    Q_OBJECT
private:
    QTableWidget *table;
    QCPBarsGroup *bargroup;


public:
    postmarkChart(QTableWidget *table, QWidget *parent = 0);
    void update();

signals:

public slots:

};

#endif // POSTMARK_CHART

