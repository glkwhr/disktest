#include "iozone_paramwidget.h"
#include <QDebug>

iozoneParamWidget::iozoneParamWidget(QWidget *parent):QWidget(parent)
{
    /* 初始化变量 */
    iFileSize = 16;
    qsConfigDir = tr("./config.ini");
    bFlagRun = false;

    /* 如果当前目录没有配置文件 则新生成一个 */
    if(!QFile::exists(qsConfigDir)){
        qsetConfig = new QSettings(tr("config.ini"), QSettings::IniFormat);

        qsetConfig->beginGroup("testConfig");
            qsetConfig->setValue("filesize", 16);
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
    }
    else{
        /* 读取ini文件 */
        qsetConfig = new QSettings(tr("config.ini"), QSettings::IniFormat);
        //qDebug()<<"read"<<endl;
    }
    /* 从当前目录下的ini文件读取配置信息 */
    qsetConfig->beginGroup("testConfig");
        iFileSize = qsetConfig->value("filesize").toInt();
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

    iTotalTimes = (int)log2((double)iFileSize*1024) - 1;
    if(iTotalTimes > 13) iTotalTimes = 13;/* 用于进度条控制 */

    /* 配置文件选择 */
    btnConfigDir.setText(tr("..."));

    /* 文件大小选择 */
    labelFileSize.setText(QString::number(iFileSize )+tr(" M"));
    labelFileSize.setAlignment(Qt::AlignLeft);
    sliderFileSize.setOrientation(Qt::Horizontal);
    sliderFileSize.setRange(0, 4);/* 16 32 64 128 256 */
    sliderFileSize.setValue((int)log2((double)iFileSize)-4);/* 当前值转到slider上 */
    connect(&sliderFileSize, SIGNAL(valueChanged(int)), this, SLOT(onFileSizeChanged(int)));

    QGridLayout * gridlayout = new QGridLayout;
    gridlayout->addWidget(&labelFileSize, 0, 0);
    gridlayout->addWidget(&sliderFileSize, 0, 1);

    this->setLayout(gridlayout);
}


struct iozoneParamStruct * iozoneParamWidget::getParamData(int type)
{
    struct iozoneParamStruct *res = new iozoneParamStruct;
    res->iFileSize = iFileSize;
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
    labelFileSize.setText(QString::number(iFileSize)+tr(" M"));
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
