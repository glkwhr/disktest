#include "postmark_thread.h"

extern "C" {
    int postmark_main(int nowfs, int argc, char *argv[32]);

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

    //ramfs
    if (whichfs & 4)
    {
        pgs->setVisible(true);
        qsetConfig->beginGroup("ramfs");
            qsRamfsDevDir = qsetConfig->value("dev").toString();
            qsRamfsMntDir = qsetConfig->value("mnt").toString();
            qsRamfsFsType = qsetConfig->value("fstype").toString();
        qsetConfig->endGroup();

        //挂载ramfs
        label->setText("mount ramfs...");
        lineMount = "mkdir";
        args.clear();
        args.append(qsRamfsMntDir);
        QProcess::execute(lineMount, args);

        lineMount = "mount";
        args.clear();
        args.append("-t");
        args.append(qsRamfsFsType);
        args.append(qsRamfsDevDir);
        args.append(qsRamfsMntDir);
        QProcess::execute(lineMount, args);
        sprintf(argv[0], "set location %s", qsRamfsMntDir.toStdString().c_str() );
        label->setText("Testing ramfs...");

        postmark_main(0, argc, argv);

        label->setText("umount ramfs...");
        //umount
        lineMount = "umount";
        args.clear();
        args.append(qsRamfsMntDir);
        QProcess::execute(lineMount, args);

    }


    //obfs
    if (whichfs & 2)
    {
        pgs->setVisible(true);
        qsetConfig->beginGroup("obfs");
            qsRamfsDevDir = qsetConfig->value("dev").toString();
            qsRamfsMntDir = qsetConfig->value("mnt").toString();
            qsRamfsFsType = qsetConfig->value("fstype").toString();
        qsetConfig->endGroup();
        //挂载obfs
        label->setText("mount obfs...");
        lineMount = "mkdir";
        args.clear();
        args.append(qsRamfsMntDir);
        QProcess::execute(lineMount, args);

        lineMount = "mount";
        args.clear();
        args.append("-t");
        args.append(qsRamfsFsType);
        args.append(qsRamfsDevDir);
        args.append(qsRamfsMntDir);
        QProcess::execute(lineMount, args);
        sprintf(argv[0], "set location %s", qsRamfsMntDir.toStdString().c_str() );
        label->setText("Testing obfs...");

        postmark_main(1, argc, argv);

        label->setText("umount obfs...");
        //umount
        lineMount = "umount";
        args.clear();
        args.append(qsRamfsMntDir);
        QProcess::execute(lineMount, args);
    }


    //pmfs
    if (whichfs & 1)
    {
        pgs->setVisible(true);
        qsetConfig->beginGroup("pmfs");
            qsRamfsDevDir = qsetConfig->value("dev").toString();
            qsRamfsMntDir = qsetConfig->value("mnt").toString();
            qsRamfsFsType = qsetConfig->value("fstype").toString();
        qsetConfig->endGroup();

        //挂载pmfs
        label->setText("mount pmfs...");
        lineMount = "mkdir";
        args.clear();
        args.append(qsRamfsMntDir);
        QProcess::execute(lineMount, args);

        lineMount = "mount";
        args.clear();
        args.append("-t");
        args.append(qsRamfsFsType);
        args.append(qsRamfsDevDir);
        args.append(qsRamfsMntDir);
        QProcess::execute(lineMount, args);
        sprintf(argv[0], "set location %s", qsRamfsMntDir.toStdString().c_str() );
        label->setText("Testing pmfs...");

        postmark_main(2, argc, argv);

        label->setText("umount pmfs...");
        //umount
        lineMount = "umount";
        args.clear();
        args.append(qsRamfsMntDir);
        QProcess::execute(lineMount, args);
    }

    pgs->setVisible(false);
    label->setText("");



    for (int i = 0; i < 32; ++i)
    {
        delete[] argv[i];
    }
}
