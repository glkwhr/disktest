#ifndef IOZONE_WIDGET
#define IOZONE_WIDGET

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>

#include "iozone_paramwidget.h"
#include "iozone_thread.h"
#include "iozone_event.h"
#include "iozone_chart.h"

#ifndef FILESYS_TYPE
#define FILESYS_TYPE

#define FILESYS_TYPE_RAMFS 0
#define FILESYS_TYPE_OBFS 1
#define FILESYS_TYPE_PMFS 2

#endif

#ifndef OUTPUT_TYPE
#define OUTPUT_TYPE
#define OUTPUT_TYPE_WRITE 0
#define OUTPUT_TYPE_REWRITE 1
#define OUTPUT_TYPE_READ 2
#define OUTPUT_TYPE_REREAD 3
#define OUTPUT_TYPE_RANDREAD 4
#define OUTPUT_TYPE_RANDWRITE 5
#endif

class iozoneWidget : public QWidget
{
    Q_OBJECT
public slots:
    void onStartRamfs();
    void onStartObfs();
    void onStartPmfs();
    //void onRateTypeChanged(const int);

public:
    iozoneWidget(QWidget * parent);
    void myEventHandle(QEvent * e);

private:
    QPushButton *btnStartRamfs;
    QPushButton *btnStartObfs;
    QPushButton *btnStartPmfs;
    QTableWidget *tableIozoneLog;
    QCheckBox *chkFlagIozoneLog;
    QProgressBar *progressBar;
    QComboBox *cboRateType;
    bool bFlagIozoneLog; //设置是否显示log窗口
    int iCurFsType; /* 当前测试文件系统类型 */

    iozoneChart *chartView;/* 图表部件 */
    iozoneRateModel *rateModel;/* 数据模型 */

    iozoneParamWidget * paramWidget; /* 各种变量信息的获取 */
};

#endif // IOZONE_WIDGET

