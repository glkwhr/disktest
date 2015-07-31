#include "postmark/postmark_thread.h"

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


    QString devDir;/* 设备路径 */
    QString mntDir;/* 挂载路径 */
    QString fsType;/* 文件系统类型 */


    QString command = "mount";
    QStringList args;

    bool useShellToMount = qsetConfig->value("shell").toBool();
    QString shellDir = qsetConfig->value("shelldir").toString();


    //开始postmark的测试

    extern char global_fschararray[FS_NUM][16];
    for (int i = 0; i < FS_NUM; ++i)
    {
        char buff[32];
        if (whichfs & (1 << i))
        {
            pgs->setVisible(true);
            qsetConfig->beginGroup(global_fschararray[i]);
                devDir = qsetConfig->value("dev").toString();
                mntDir = qsetConfig->value("mnt").toString();
                fsType = qsetConfig->value("fstype").toString();
            qsetConfig->endGroup();

            if (useShellToMount)
            {
                /* 执行shell 以"sh ./shell fstype mount"格式 */
                command = "sh";
                args.clear();
                args << shellDir << fsType << "mount";
                QProcess::execute( command, args );
            }
            else
            {
                //mkfs
                label->setText("Formatting Device ...");
                command = "mkfs";
                args.clear();
                args.append("-t");
                args.append(fsType);
                args.append(devDir);
                QProcess::execute(command, args);

                //挂载
                sprintf(buff, "Mounting %s ...", global_fschararray[i]);
                label->setText(buff);
                command = "mount";
                args.clear();
                args.append("-t");
                args.append(fsType);
                args.append(devDir);
                args.append(mntDir);
                QProcess::execute(command, args);
                sprintf(argv[0], "set location %s", mntDir.toStdString().c_str() );
                sprintf(buff, "Testing %s ...", global_fschararray[i]);
                label->setText(buff);
            }

            sleep(1);/* 给时间完成以上工作 */


            //Postmark
            postmark_main(i, argc, argv);


            //umount
            if (useShellToMount)
            {
                /* 执行shell 以"sh ./shell fstype umount"格式 */
                command = "sh";
                args.clear();
                args << shellDir << fsType << "umount";
                QProcess::execute( command, args );
            }
            else
            {
                sprintf(buff, "Umounting %s ...", global_fschararray[i]);
                label->setText(buff);
                command = "umount";
                args.clear();
                args.append(mntDir);
                QProcess::execute(command, args);
            }

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
