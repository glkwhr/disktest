#ifndef IOZONE_WIDGET
#define IOZONE_WIDGET

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QCheckBox>
#include <QGridLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QProgressBar>

#include "iozone_paramwidget.h"

#define OUTPUT_TYPE_WRITE 0
#define OUTPUT_TYPE_REWRITE 1
#define OUTPUT_TYPE_READ 2
#define OUTPUT_TYPE_REREAD 3
#define OUTPUT_TYPE_RANDREAD 4
#define OUTPUT_TYPE_RANDWRITE 5

class iozoneWidget : public QWidget
{
    Q_OBJECT
public slots:
    void onStartRamfs();
    void onStartObfs();
    void onStartPmfs();

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
    bool bFlagIozoneLog; //设置是否显示log窗口

    iozoneParamWidget * paramWidget; /* 各种变量信息的获取 */
};

#endif // IOZONE_WIDGET

