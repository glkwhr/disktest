#include "iozone_thread.h"
#include "paintedwidget.h"

#include <QDebug>

#include <cstdio>

extern "C" {
    int iozoneMain(struct iozoneThread* p, int argc,char *argv[32]);
}

void notifyGUI(int type, long long kb, long long reclen, unsigned long long speed);

extern "C" void callNotify(iozoneThread* p, int type, long long kb, long long reclen, unsigned long long speed) // wrapper function
{   /* 子进程的iozoneMain()中调用多次 */
    p->threadNotifyGUI(p, type, kb, reclen, speed);
}

void iozoneThread::threadNotifyGUI(iozoneThread *p, int type, long long kb, long long reclen, unsigned long long speed){
    /* 子进程中调用,用来与父进程通信 */
    while(pShmNotify->notifyFlag == 1);/* 等待父进程处理之前的请求 */
    if(pShmNotify->notifyFlag == 0)
    {   /* 之前请求已处理 */
        pShmNotify->type = type;
        pShmNotify->kb = kb;
        pShmNotify->reclen = reclen;
        pShmNotify->speed = speed;
        pShmNotify->notifyFlag = 1; /* "通知"父进程 */
    }
}

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

    /*
    strcpy(argv[i++], "-Rb");
    strcpy(argv[i], "res");
    strcat(argv[i], tagDateTime.toStdString().c_str());
    strcat(argv[i++], ".xls");
    */
    argc = i;

    /*调用iozone主函数*/
    //while(i-->0) qDebug() << argv[i]; qDebug() << endl;
    //iozoneMain(this, argc, argv);

    iozoneThread *p = this;
    pid_t pid;
    i = 0;
    goFork:
    if((pid = fork())==0){
        /* 子进程 */
        /* 共享内存 */
        shmid = shmget( ftok(".", 1), sizeof(struct shmNotify), 0666|IPC_CREAT);
        if(shmid == -1)
        {
            exit(EXIT_FAILURE);
        }
        shm = shmat(shmid, (void*)0, 0);
        if(shm == (void*)-1)
        {
            exit(EXIT_FAILURE);
        }
        qDebug()<<"Memory attached at "<<shm<<endl;
        pShmNotify = (struct shmNotify*)shm;
        if(pShmNotify->notifyFlag != 0)
            pShmNotify->notifyFlag = 0;
        iozoneMain(p, argc, argv);
        while(pShmNotify->notifyFlag == 1);/* 等待父进程处理完毕 */
        pShmNotify->notifyFlag = -1;/* 结束标志 */
        /* 把共享内存从当前进程中分离 */
        if(shmdt(pShmNotify) == -1)
        {
            qDebug()<<"shmdt failed"<<endl;
            exit(EXIT_FAILURE);
        }
        exit();
    }
    else if(pid>0){
        /* 父进程中 */
        ++i;
        /* 共享内存 */
        shmid = shmget( ftok(".", 1), sizeof(struct shmNotify), 0666|IPC_CREAT);
        if(shmid == -1)
        {
            exit(EXIT_FAILURE);
        }
        shm = shmat(shmid, (void*)0, 0);
        if(shm == (void*)-1)
        {
            exit(EXIT_FAILURE);
        }
        qDebug()<<"Memory attached at "<<shm<<endl;
        pShmNotify = (struct shmNotify*)shm;
        if(pShmNotify->notifyFlag != 0&&(pShmNotify->notifyFlag!=1))
            pShmNotify->notifyFlag = 0;
        while(1)
        {
            if(pShmNotify->notifyFlag == 1)
            {
                notifyGUI(pShmNotify->type, pShmNotify->kb, pShmNotify->reclen, pShmNotify->speed);
                pShmNotify->notifyFlag = 0;
                continue;
            }
            else if(pShmNotify->notifyFlag == -1)
            {
                pShmNotify->notifyFlag = 0;
                break;
            }
        }
        waitpid(pid, NULL, 0);/* 等待子进程 */
        if ( i<param->iTestTimes ) goto goFork;
        /* 把共享内存从当前进程分离 */
        if(shmdt(pShmNotify) == -1)
        {
            qDebug()<<"shmdt failed"<<endl;
            exit(EXIT_FAILURE);
        }
        /* 取消挂载 *//*
        lineMount = "umount";
        args.clear();
        args.append(param->qsMntDir);
        QProcess::execute(lineMount, args);*/
        /* 测试结束 */
        *(param->pbFlagRun) = false;

        /* 释放内存 */
        for ( i = 0; i < DEFAULT_ARGV; ++i)
        {
                delete argv[i];
        }
        delete this->param;/* 在点击start按钮时getparam()中new的 */
        this->quit();
    }
    //this->deleteLater();
}



