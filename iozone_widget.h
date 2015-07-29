#ifndef IOZONE_WIDGET
#define IOZONE_WIDGET

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QComboBox>

#include "iozone_paramwidget.h"
#include "iozone_thread.h"
#include "iozone_event.h"
#include "iozone_chart.h"

#ifndef FILESYS_TYPE
#define FILESYS_TYPE
#define FILESYS_COUNT 3
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
    void onStartTest();
    void startNextTest();
    //void onStartObfs();
    //void onStartPmfs();
    //void onRateTypeChanged(const int);

public:
    iozoneWidget(QWidget * parent);
    void myEventHandle(QEvent * e);

private:
    QPushButton *btnStartTest;
    //QPushButton *btnStartObfs;
    //QPushButton *btnStartPmfs;
    QTableWidget *tableIozoneLog;
    QCheckBox *chkFlagsFsToTest[FILESYS_COUNT];
    //QCheckBox *chkFlagObfs;
    //QCheckBox *chkFlagPmfs;
    QCheckBox *chkFlagIozoneLog;
    QProgressBar *progressBar;
    QLabel *labelStatus;
    QComboBox *cboRateType;
    QStringList qslFsType;
    bool bFlagIozoneLog; //设置是否显示log窗口
    int iCurFsType; /* 当前测试文件系统类型 */

    /* 当前需要测试的文件系统, 二进制从低到高各位表示不同文件系统
     * 点击start时从checkboxs中获取初值, 每测完一个文件系统更新一次 */
    int iFsToTest;

    iozoneChart *chartView;/* 图表部件 */
    iozoneRateModel *rateModel;/* 数据模型 */

    iozoneParamWidget * paramWidget; /* 各种变量信息的获取 */
};

#endif // IOZONE_WIDGET

