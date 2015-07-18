#include "iozone_thread.h"
#include "paintedwidget.h"

#include <QDebug>


extern "C" {
    int iozoneMain(int argc, char *argv[32]);
}

#include <cstdio>

void iozoneThread::run()
{
    int argc = DEFAULT_ARGC;
    int i = 0;
    char *argv[DEFAULT_ARGV];
    for( ; i<DEFAULT_ARGV; ++i)
        argv[i] = new char[DEFAULT_ARG_CHAR];

    /* 挂载文件系统 *//*
    QString lineMount = "mount";
    QStringList args;
    args.append("-t");
    args.append(param->qsFsType);
    args.append(param->qsDevDir);
    args.append(param->qsMntDir);
    QProcess::execute(lineMount, args);*/

    //获取系统时间并设置显示格式
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString tagDateTime = currentDateTime.toString("yyyyMMddhhmmss");

    /*生成字符串数组argv*/
    i = 0;

    strcpy(argv[i++], "./iozone");

    if(param->bFlaga)
        strcpy(argv[i++], "-a");

    if(param->bFlags) {
        strcpy(argv[i++], "-s");
        sprintf(argv[i++], "%dm", param->iFileSize);
    }
    if(param->bFlagi0){
        strcpy(argv[i++], "-i");
        strcpy(argv[i++], "0");
    }
    if(param->bFlagi1){
        strcpy(argv[i++], "-i");
        strcpy(argv[i++], "1");
    }
    if(param->bFlagi2){
        strcpy(argv[i++], "-i");
        strcpy(argv[i++], "2");
    }
    strcpy(argv[i++], "-f");
    strcpy(argv[i], param->qsFileName.toStdString().c_str() );
    strcat(argv[i++], tagDateTime.toStdString().c_str());

    strcpy(argv[i++], "-Rb");
    strcpy(argv[i], "res");
    strcat(argv[i], tagDateTime.toStdString().c_str());
    strcat(argv[i++], ".xls");

    argc = i;

    /*调用iozone主函数*/
    //while(i-->0) qDebug() << argv[i]; qDebug() << endl;
    iozoneMain(argc, argv);
    //qDebug() << "1" << endl;


    /* 取消挂载 *//*
    lineMount = "umount";
    args.clear();
    args.append(param->qsMntDir);
    QProcess::execute(lineMount, args);*/

    for ( i = 0; i < argc; ++i)
    {
            delete[] argv[i];
    }

    /* 测试结束 */
    *(param->pbFlagRun) = false;
}



