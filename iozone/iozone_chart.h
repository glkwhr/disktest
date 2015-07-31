#ifndef IOZONE_CHART_H
#define IOZONE_CHART_H

#include "qcustomplot/qcustomplot.h"
#include "iozone/iozone_rate_model.h"

#define MINIMUM_DATA_RANGE 500000

#ifndef FILESYS_TYPE
#define FILESYS_TYPE
#define FILESYS_COUNT 4
#define FILESYS_TYPE_RAMFS 0
#define FILESYS_TYPE_OBFS 1
#define FILESYS_TYPE_PMFS 2
#define FILESYS_TYPE_EXT4 3
#endif


/* 看作chartview */
class iozoneChart : public QCustomPlot//, public QFrame
{
    Q_OBJECT
public:
    explicit iozoneChart(QFrame *frame, QWidget *parent = 0);
    void setModel(int rateType, iozoneRateModel *model);/* chart按ratetype分类 */

public slots:
    void update(void);/* 更新图中数据 */
    void setRateType(const int rateType);

private:
    iozoneRateModel *dataModel;
    int iRateType; /* 该chart的显示内容 */
    QStringList qslFsName;
    QStringList qslRateName;
    QVector<double> ticks;
    QVector<QString> labels;

};

#endif // IOZONE_CHART_H
