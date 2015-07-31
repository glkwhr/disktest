#include "iozone/iozone_paramwidget.h"
#include <QDebug>

iozoneParamWidget::iozoneParamWidget(QWidget *parent):QWidget(parent)
{
    /* 初始化变量 */
    iFileSize = DEFAULT_CONFIG_FILESIZE;
    iTestTimes = DEFAULT_CONFIG_TESTTIMES;
    qsConfigDir = tr("./config.ini");
    bFlagRun = false;

    /* 如果当前目录没有配置文件 则新生成一个 */
    if(!QFile::exists(qsConfigDir)){
        qsetConfig = new QSettings(tr("config.ini"), QSettings::IniFormat);

        qsetConfig->beginGroup("testConfig");
            qsetConfig->setValue("shell", false);/* 是否使用shell执行 */
            qsetConfig->setValue("shelldir", "./shell.sh");/* 默认shell位置 */
            qsetConfig->setValue("filesize", DEFAULT_CONFIG_FILESIZE);
            qsetConfig->setValue("testtimes", DEFAULT_CONFIG_TESTTIMES);
            qsetConfig->setValue("mkfs", false);/* 是否自动格式化 */
            qsetConfig->setValue("mount", false); /* 是否自动挂载/卸载 */
            qsetConfig->setValue("flaga", true);
            qsetConfig->setValue("flags", true);
            qsetConfig->setValue("flagi0", true);
            qsetConfig->setValue("flagi1", true);
            qsetConfig->setValue("flagi2", true);
        qsetConfig->endGroup();
        qsetConfig->beginGroup("ramfs");
            qsetConfig->setValue("filename", "/ramfs/tmpfile");
            qsetConfig->setValue("dev", "/dev/ram0");
            qsetConfig->setValue("mnt", "/ramfs");
            qsetConfig->setValue("fstype", "ext3");
        qsetConfig->endGroup();
        qsetConfig->beginGroup("obfs");
            qsetConfig->setValue("filename", "/obfs/tmpfile");
            qsetConfig->setValue("dev", "/dev/obfsdev");
            qsetConfig->setValue("mnt", "/obfs");
            qsetConfig->setValue("fstype", "obfs");
        qsetConfig->endGroup();
        qsetConfig->beginGroup("pmfs");
            qsetConfig->setValue("filename", "/pmfs/tmpfile");
            qsetConfig->setValue("dev", "/dev/pmfsdev");
            qsetConfig->setValue("mnt", "/pmfs");
            qsetConfig->setValue("fstype", "pmfs");
        qsetConfig->endGroup();
        qsetConfig->beginGroup("ext4");
            qsetConfig->setValue("filename", "/ext4/tmpfile");
            qsetConfig->setValue("dev", "/dev/ext4");
            qsetConfig->setValue("mnt", "/ext4");
            qsetConfig->setValue("fstype", "ext4");
        qsetConfig->endGroup();
    }
    else{
        /* 读取ini文件 */
        qsetConfig = new QSettings(tr("config.ini"), QSettings::IniFormat);
        //qDebug()<<"read"<<endl;
    }
    /* 从当前目录下的ini文件读取配置信息 */
    qsetConfig->beginGroup("testConfig");
        bFlagShell = qsetConfig->value("shell").toBool();
        qsShellDir = qsetConfig->value("shelldir").toString();
        iFileSize = qsetConfig->value("filesize").toInt();
        iTestTimes = qsetConfig->value("testtimes").toInt();
        bFlagMkfs = qsetConfig->value("mkfs").toBool();/* 是否自动格式化 */
        bFlagMnt = qsetConfig->value("mount").toBool();
        bFlaga = qsetConfig->value("flaga").toBool();
        bFlags = qsetConfig->value("flags").toBool();
        bFlagi0 = qsetConfig->value("flagi0").toBool();
        bFlagi1 = qsetConfig->value("flagi1").toBool();
        bFlagi2 = qsetConfig->value("flagi2").toBool();
    qsetConfig->endGroup();
    qsetConfig->beginGroup("ramfs");
        qsRamfsFileName = qsetConfig->value("filename").toString();
        qsRamfsDevDir = qsetConfig->value("dev").toString();
        qsRamfsMntDir = qsetConfig->value("mnt").toString();
        qsRamfsFsType = qsetConfig->value("fstype").toString();
    qsetConfig->endGroup();
    qsetConfig->beginGroup("obfs");
        qsObfsFileName = qsetConfig->value("filename").toString();
        qsObfsDevDir = qsetConfig->value("dev").toString();
        qsObfsMntDir = qsetConfig->value("mnt").toString();
        qsObfsFsType = qsetConfig->value("fstype").toString();
    qsetConfig->endGroup();
    qsetConfig->beginGroup("pmfs");
        qsPmfsFileName = qsetConfig->value("filename").toString();
        qsPmfsDevDir = qsetConfig->value("dev").toString();
        qsPmfsMntDir = qsetConfig->value("mnt").toString();
        qsPmfsFsType = qsetConfig->value("fstype").toString();
    qsetConfig->endGroup();
    qsetConfig->beginGroup("ext4");
        qsExt4FileName = qsetConfig->value("filename").toString();
        qsExt4DevDir = qsetConfig->value("dev").toString();
        qsExt4MntDir = qsetConfig->value("mnt").toString();
        qsExt4FsType = qsetConfig->value("fstype").toString();
    qsetConfig->endGroup();

    iTotalTimes = (int)log2((double)iFileSize*1024) - 1;
    if(iTotalTimes > 13) iTotalTimes = 13;/* 用于进度条控制 */

    /* 配置文件选择 */
    btnConfigDir.setText(tr("..."));

    /* 文件大小选择 */
    labelFileSize.setText(tr("File Size ")+QString::number(iFileSize )+tr(" M"));
    labelFileSize.setAlignment(Qt::AlignLeft);
    sliderFileSize.setOrientation(Qt::Horizontal);
    sliderFileSize.setRange(0, 4);/* 16 32 64 128 256 */
    sliderFileSize.setValue((int)log2((double)iFileSize)-4);/* 当前值转到slider上 */
    connect(&sliderFileSize, SIGNAL(valueChanged(int)), this, SLOT(onFileSizeChanged(int)));
    /* 测试次数选择 */
    labelTestTimes.setText(tr("Test Times "));
    labelTestTimes.setAlignment(Qt::AlignLeft);
    qsbTestTimes.setSingleStep(1);
    qsbTestTimes.setRange(1,100);
    qsbTestTimes.setValue(iTestTimes);
    connect(&qsbTestTimes, SIGNAL(valueChanged(int)), this, SLOT(onTestTimesChanged(int)));

    QGridLayout * gridlayout = new QGridLayout(this);
    gridlayout->addWidget(&labelFileSize, 0, 0);
    gridlayout->addWidget(&sliderFileSize, 0, 1);
    gridlayout->addWidget(&labelTestTimes, 1, 0);
    gridlayout->addWidget(&qsbTestTimes, 1, 1);
    gridlayout->setMargin(0);
    this->setLayout(gridlayout);
}


struct iozoneParamStruct * iozoneParamWidget::getParamData(int type)
{
    struct iozoneParamStruct *res = new iozoneParamStruct;
    res->iCurFsType = type;
    res->bFlagShell = bFlagShell;
    res->qsShellDir = qsShellDir;
    res->iFileSize = iFileSize;
    res->iTestTimes = iTestTimes;
    res->bFlagMkfs = bFlagMkfs;
    res->bFlagMnt = bFlagMnt;
    res->bFlaga = bFlaga;
    res->bFlags = bFlags;
    res->bFlagi0 = bFlagi0;
    res->bFlagi1 = bFlagi1;
    res->bFlagi2 = bFlagi2;
    res->pbFlagRun = &bFlagRun;
    switch (type) {
    case FILESYS_TYPE_RAMFS:
        res->qsFileName = qsRamfsFileName;
        res->qsDevDir = qsRamfsDevDir;
        res->qsMntDir = qsRamfsMntDir;
        res->qsFsType = qsRamfsFsType;
        break;
    case FILESYS_TYPE_OBFS:
        res->qsFileName = qsObfsFileName;
        res->qsDevDir = qsObfsDevDir;
        res->qsMntDir = qsObfsMntDir;
        res->qsFsType = qsObfsFsType;
        break;
    case FILESYS_TYPE_PMFS:
        res->qsFileName = qsPmfsFileName;
        res->qsDevDir = qsPmfsDevDir;
        res->qsMntDir = qsPmfsMntDir;
        res->qsFsType = qsPmfsFsType;
        break;
    case FILESYS_TYPE_EXT4:
        res->qsFileName = qsExt4FileName;
        res->qsDevDir = qsExt4DevDir;
        res->qsMntDir = qsExt4MntDir;
        res->qsFsType = qsExt4FsType;
        break;
    default:
        break;
    }
    return res;
}

void iozoneParamWidget::setRunFlag(bool bCurrentStatus)
{
    bFlagRun = bCurrentStatus;
}

void iozoneParamWidget::onFileSizeChanged(int iSetFileSize)
{
    iFileSize = 16;
    for(int i = 1; i<=iSetFileSize; i++)
        iFileSize *= 2;
    iTotalTimes = (int)log2((double)iFileSize*1024) - 1;
    if(iTotalTimes>13) iTotalTimes = 13; /*用于之后的进度条操作*/
    labelFileSize.setText(tr("File Size ")+QString::number(iFileSize)+tr(" M"));
}

void iozoneParamWidget::onTestTimesChanged(int iSetTestTimes)
{
    iTestTimes = iSetTestTimes;
}

/* 修改配置文件目录 */
void iozoneParamWidget::onConfigDirClicked(void)
{
    if(bFlagRun){
            return;
    }
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Choose config file"));
    fileDialog->setDirectory(qsConfigDir);
    fileDialog->setFilter(QDir::Files);
    fileDialog->setFileMode(QFileDialog::ExistingFile);/* 可能会选到非ini文件 在后面进行判断 */
    if(fileDialog->exec() == QDialog::Accepted) {
        QString qstmp = fileDialog->selectedFiles()[0];
         int index = qstmp.lastIndexOf(".");
        QString qsFileExt = qstmp.right(qstmp.length() - index) ;/* 获得选中文件扩展名 */
        if( qsFileExt != tr("ini")){
            /* 选中的不是ini文件 */
            QMessageBox::warning(this, tr("ERROR"), tr("Wrong config file type!"));
            return;
        }
        qsConfigDir = fileDialog->selectedFiles()[0];
        lineConfigDir.setText(qsConfigDir);
        //QMessageBox::information(NULL, tr("Path"), tr("You selected ") + qsFileDir);
    } else {
            //QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    }
}

void iozoneParamWidget::onTestEnded(void)
{
    bFlagRun = false;
}
