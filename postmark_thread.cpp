#include "postmark_thread.h"

extern "C" {
    int postmark_main(int nowfs, int argc, char *argv[32]);
    extern void notifyGUI_2_int(int nowfs, int isprogress, int type, int total, int persecond);
}

#include "postmark_paramwidget.h"
#include <cstdio>
#include <QSettings>

#include <QSettings>
#include <QFile>

#include <QProcess>



void postmarkThread::run()
{
    int argc = 8;
    char *argv[32];
    for (int i = 0; i < 32; ++i)
    {
        argv[i] = new char[32];
    }

    /*
        set location /pmfs
        set size 50 10000
        set read 50
        set write 50
        set buffering false
        set number 1000
        set transactions 50000
        run
    */


    int i = -1;

    ++i;
    //sprintf(argv[++i], "set location %s", param->file_posi.toStdString().c_str() );

    sprintf(argv[++i], "set size %d %d", param->file_size_min, param->file_size_max);

    sprintf(argv[++i], "set read %d", param->read_block_size);
    sprintf(argv[++i], "set write %d", param->write_block_size);

    if (param->use_buffer_checked)
    {
        strcpy(argv[++i], "set buffering false");
    }
    else
    {
        strcpy(argv[++i], "set buffering false");
    }

    sprintf(argv[++i], "set number %d", param->number);

    sprintf(argv[++i], "set transactions %d", param->transactions);

    strcpy(argv[++i], "run");


    delete param;

    QString qsConfigDir("./config.ini");
    QSettings * qsetConfig;
    if(!QFile::exists(qsConfigDir))
    {
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
            qsetConfig->setValue("fstype", "ramfs");
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


    QString qsRamfsDevDir;/* 设备路径 */
    QString qsRamfsMntDir;/* 挂载路径 */
    QString qsRamfsFsType;/* 文件系统类型 */


    QString lineMount = "mount";
    QStringList args;
    //开始postmark的测试

    extern char global_fschararray[FS_NUM][16];
    for (int i = 0; i < FS_NUM; ++i)
    {
        char buff[32];
        if (whichfs & (1 << i))
        {
            pgs->setVisible(true);
            qsetConfig->beginGroup(global_fschararray[i]);
                qsRamfsDevDir = qsetConfig->value("dev").toString();
                qsRamfsMntDir = qsetConfig->value("mnt").toString();
                qsRamfsFsType = qsetConfig->value("fstype").toString();
            qsetConfig->endGroup();

            //mkfs
            label->setText("Formatting Device ...");
            lineMount = "mkfs";
            args.clear();
            args.append("-t");
            args.append(qsRamfsFsType);
            args.append(qsRamfsDevDir);
            QProcess::execute(lineMount, args);

            //挂载
            sprintf(buff, "Mounting %s ...", global_fschararray[i]);
            label->setText(buff);
            lineMount = "mount";
            args.clear();
            args.append("-t");
            args.append(qsRamfsFsType);
            args.append(qsRamfsDevDir);
            args.append(qsRamfsMntDir);
            QProcess::execute(lineMount, args);
            sprintf(argv[0], "set location %s", qsRamfsMntDir.toStdString().c_str() );
            sprintf(buff, "Testing %s ...", global_fschararray[i]);
            label->setText(buff);

            //Postmark
            postmark_main(i, argc, argv);

            //umount
            sprintf(buff, "Umounting %s ...", global_fschararray[i]);
            label->setText(buff);
            lineMount = "umount";
            args.clear();
            args.append(qsRamfsMntDir);
            QProcess::execute(lineMount, args);
        }
    }

    //send thread end signal
    notifyGUI_2_int(FS_NUM-1, 4, 0, 0, 0);

    pgs->setVisible(false);
    label->setText("");



    for (int i = 0; i < 32; ++i)
    {
        delete[] argv[i];
    }
}
