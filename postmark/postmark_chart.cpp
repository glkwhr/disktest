#include "postmark/postmark_chart.h"
#include "postmark/postmark_thread.h"

postmarkChart::postmarkChart(int k, QTableWidget *tbl, QWidget *parent):QCustomPlot(parent), table(tbl)
{
    kind = k;
    for (int i = 0; i < FS_NUM; ++i)
    {

        bars[i] = 0;
    }
    QVector<QString> labels(12);
    if (k == 0)
    {
        int i = -1;
        labels[++i] = "     ";
        labels[++i] = "Create";
        labels[++i] = "Read";
        labels[++i] = "Append";
        labels[++i] = "Delete";

    }
    else
    {
        int i = -1;
        labels[++i] = "     ";
        labels[++i] = "Read data";
        labels[++i] = "Write data";
    }


    bargroup = new QCPBarsGroup(this);
    bargroup->setSpacing(1);

    if (kind == 0)
    {
        this->yAxis->setLabel("Rate (files per second)");
    }
    else
    {
        this->yAxis->setLabel("Rate (megabytes per second)");
    }



    //this->xAxis->setAutoTicks(false);
    this->xAxis->setAutoTickLabels(false);
    this->xAxis->setTickVectorLabels(labels);

    this->legend->setVisible(true);
    this->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
    this->legend->setBrush(QColor(255, 255, 255, 200));


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
    //plotGradient.setFinalStop(0, this->geometry().height());
    plotGradient.setFinalStop(0, 480);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    //axisRectGradient.setFinalStop(0, this->geometry().height());
    int ttt = this->geometry().height();
    axisRectGradient.setFinalStop(0, 480);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    this->axisRect()->setBackground(axisRectGradient);
    this->setBackground(plotGradient);




}

void postmarkChart::update()
{


    int color[10][4] = {{0, 0, 255, 50},
                        {180, 0, 120, 50},
                        {255, 154, 0, 50},
                        {0, 255, 0, 50},
                        {255, 0, 0, 50}
                       };

    QVector<double> datax;
    if (kind == 0)
    {
         datax << 1 << 2 << 3 << 4 ;
    }
    else
    {
        datax << 1 << 2;
    }


    double max1;
    double max2;
    double tmp;
    max1 = 0;
    max2 = 0;

    for (int i = 0; i < FS_NUM; ++i)
    {
        if (table->item(i, 0) == 0)
        {
            continue;
        }
        QVector<double> datay = QVector<double>();
        datay.clear();
        if (kind == 0)
        {
            for (int j = 0; j < 4; ++j)
            {
                tmp = table->item(i, j)->text().toDouble();
                datay << tmp;
                if (tmp > max1)
                    max1 = tmp;

            }
        }
        else
        {
            for (int j = 4; j < 6; ++j)
            {
                QString t(table->item(i, j)->text());
                QStringList list = t.split(" ");

                tmp = list[0].toDouble();
                datay << tmp;
                if (tmp > max2)
                    max2 = tmp;
            }
        }

        if (bars[i] == 0)
        {
            bars[i] = new QCPBars(this->xAxis, this->yAxis);
            this->addPlottable(bars[i]);
        }
        bars[i]->setData(datax, datay);
        bars[i]->setBrush(QColor(color[i][0], color[i][1], color[i][2], color[i][3]));
        bars[i]->setPen(QColor(color[i][0], color[i][1], color[i][2]));
        bars[i]->setWidth(0.15);
        bars[i]->setBarsGroup(bargroup);

        extern char global_fschararray[FS_NUM][16];
        bars[i]->setName(global_fschararray[i]);
    }


    if (kind == 0)
    {
        this->xAxis->setRange(0.1, 4.9);
        this->yAxis->setRange(0, max1+50);
    }
    else
    {
        this->xAxis->setRange(0.1, 2.9);
        this->yAxis->setRange(0, max2+5);
    }
    this->xAxis->setAutoTickStep(false);
    this->xAxis->setTickStep(1);

    this->replot();
}
