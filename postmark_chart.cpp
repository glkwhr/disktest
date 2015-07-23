#include "postmark_chart.h"
#include "postmark_thread.h"

postmarkChart::postmarkChart(QTableWidget *tbl, QWidget *parent):QCustomPlot(parent), table(tbl)
{
    QVector<QString> labels(6);
    labels[0] = "Create";
    labels[1] = "Read";
    labels[2] = "Append";
    labels[3] = "Delete";
    labels[4] = "Read data";
    labels[5] = "Write data";

    bargroup = new QCPBarsGroup(this);


    this->xAxis->setAutoTicks(false);
    this->xAxis->setAutoTickLabels(false);
    this->xAxis->setTickVectorLabels(labels);
}

void postmarkChart::update()
{
    int color[10][4] = {{0, 0, 255, 50},
                        {180, 0, 120, 50},
                        {255, 154, 0, 50},
                        {0, 255, 0, 50},
                        {255, 0, 0, 50}
                       };


    QVector<double> datax = QVector<double>() << 1 << 2 << 3 << 4 << 5 << 6;


    for (int i = 0; i < FS_NUM; ++i)
    {
        if (table->item(i, 0) == 0)
        {
            continue;
        }
        QVector<double> datay = QVector<double>();
        datay.clear();
        for (int j = 0; j < 6; ++j)
        {
            datay << table->item(i, j)->text().toDouble();
        }

        QCPBars *bars = new QCPBars(this->xAxis, this->yAxis);
        this->addPlottable(bars);
        bars->setData(datax, datay);
        bars->setBrush(QColor(color[i][0], color[i][1], color[i][2], color[i][3]));
        bars->setPen(QColor(color[i][0], color[i][1], color[i][2]));
        bars->setWidth(0.15);
        bars->setBarsGroup(bargroup);
    }

    this->xAxis->setRange(0.1, 6.9);
    this->yAxis->setRange(0, 5000);
    this->xAxis->setAutoTickStep(false);
    this->xAxis->setTickStep(1);

    this->replot();
}
