#ifndef IOZONE_PARAMWIDGET_H
#define IOZONE_PARAMWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

#include <cmath>

/* 文件系统的类型和数量 */
#ifndef FILESYS_TYPE
#define FILESYS_TYPE
#define FILESYS_COUNT 4
#define FILESYS_TYPE_RAMFS 0
#define FILESYS_TYPE_OBFS 1
#define FILESYS_TYPE_PMFS 2
#define FILESYS_TYPE_EXT4 3
#endif
/* 一些设置中的默认值 */
#ifndef DEFAULT_CONFIG
#define DEFAULT_CONFIG
#define DEFAULT_CONFIG_FILESIZE 16
#define DEFAULT_CONFIG_TESTTIMES 2
#endif

struct iozoneParamStruct{
    /*设置参数*/
    int iFileSize; /* 文件大小 */
    int iTestTimes;
    int iCurFsType;/* 文件系统类型 */
    bool bFlagMkfs; /* 格式化 */
    bool bFlagMnt; /* 挂载 */
    bool bFlagShell;
    bool bFlaga;
    bool bFlags;
    bool bFlagi0;
    bool bFlagi1;
    bool bFlagi2;
    bool *pbFlagRun;
    QString qsShellDir;/* shell位置 */
    QString qsFileName;
    QString qsDevDir;
    QString qsMntDir;
    QString qsFsType;/* 文件系统类型 */
};

class iozoneParamWidget : public QWidget
{
    Q_OBJECT
public:
    iozoneParamWidget(QWidget *parent = 0);
    struct iozoneParamStruct * getParamData(int);

    void setRunFlag(bool);/* 向iozoneWidget类提供设置运行状态接口 */
    bool getRunFlag(void) { return bFlagRun; }
    int getTotalTimes(void) { return iTotalTimes; }
    int getTestTimes(void) { return iTestTimes; }

public slots:
    void onFileSizeChanged(int);
    void onTestTimesChanged(int);
    void onConfigDirClicked(void);
    void onTestEnded(void);

private:
    QLabel labelFileSize;
    QLabel labelTestTimes;
    QSlider sliderFileSize;
    QSpinBox qsbTestTimes;
    QSettings *qsetConfig; /* 配置信息 */
    QLineEdit lineConfigDir;
    QPushButton btnConfigDir;

    int iFileSize;
    int iTestTimes; /* 点击一次start后一轮进行的测试次数 */
    /* iTotalTimes: 需要进行测试的次数(以iozone中不同reclen来计,主要用来控制进度条) */
    int iTotalTimes;
    bool bFlagMkfs; /* 格式化 */
    bool bFlagMnt; /* 挂载 */
    bool bFlagShell; /* 使用sh脚本实现格式化,挂载和卸载 */
    bool bFlaga;
    bool bFlags;
    bool bFlagi0;
    bool bFlagi1;
    bool bFlagi2;
    bool bFlagRun; /* 正在执行 点击start时变true thread中iozoneMain执行完后变false */
    QString qsShellDir; /* sh脚本路径 */
    QString qsRamfsFileName;
    QString qsRamfsDevDir;/* 设备路径 */
    QString qsRamfsMntDir;/* 挂载路径 */
    QString qsRamfsFsType;/* 文件系统类型 */
    QString qsObfsFileName;
    QString qsObfsDevDir;
    QString qsObfsMntDir;
    QString qsObfsFsType;
    QString qsPmfsFileName;
    QString qsPmfsDevDir;
    QString qsPmfsMntDir;
    QString qsPmfsFsType;
    QString qsExt4FileName;
    QString qsExt4DevDir;
    QString qsExt4MntDir;
    QString qsExt4FsType;
    QString qsConfigDir; //配置文件的路径
};

#endif // IOZONE_PARAMWIDGET_H

