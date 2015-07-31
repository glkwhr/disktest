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
    waitFlag->acquire();/* 若父进程等待 */
     /* 之前请求已处理 */
    pShmNotify->type = type;
    pShmNotify->kb = kb;
    pShmNotify->reclen = reclen;
    pShmNotify->speed = speed;
    pShmNotify->notifyFlag = 1; /* "通知"父进程 */
    callFlag->release();/* 子进程已通知 */
}

void iozoneThread::run()
{
    callFlag = new QSystemSemaphore("notify", 0, QSystemSemaphore::Create);
    waitFlag = new QSystemSemaphore("wait", 1, QSystemSemaphore::Create);
    shm = NULL;
    int argc = DEFAULT_ARGC;
    int i = 0;
    char *argv[DEFAULT_ARGV];
    for( ; i<DEFAULT_ARGV; ++i)
        argv[i] = new char[DEFAULT_ARG_CHAR];

    /* 格式化设备并挂载 */
    QString qsCommand;
    QStringList args;
    if ( param->bFlagMnt == true )
    {   /* 格式化设备 */
        qsCommand = "mkfs";
        args.append("-t");
        args.append(param->qsFsType);
        args.append(param->qsDevDir);
        QProcess::execute(qsCommand, args);
    }
    if ( param->bFlagMnt == true )
    {   /* 挂载文件系统 */
        qsCommand = "mount";
        args.clear();
        args.append("-t");
        args.append(param->qsFsType);
        args.append(param->qsDevDir);
        args.append(param->qsMntDir);
        QProcess::execute(qsCommand, args);
    }
    if ( param->bFlagShell == true )
    {   /* 执行shell 以"sh ./shell fstype mount"格式 */
        qsCommand = "sh";
        args.clear();
        args << param->qsShellDir << param->qsFsType << "mount";
        QProcess::execute( qsCommand, args );
    }
    sleep(1);/* 给时间完成以上工作 */

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

    argc = i;

    iozoneThread *p = this;
    pid_t pid;
    i = 0;
    goFork:
    if((pid = fork())==0){
        /* 子进程 */
        callFlag = new QSystemSemaphore("notify", 0, QSystemSemaphore::Open);
        waitFlag = new QSystemSemaphore("wait", 1, QSystemSemaphore::Open);
        /* 共享内存 */
        shmid = shmget( ftok(".", 1), sizeof(struct shmNotify), 0666|IPC_CREAT);
        if(shmid == -1)
        {
            _exit(EXIT_FAILURE);
        }
        shm = shmat(shmid, (void*)0, 0);
        if(shm == (void*)-1)
        {
            _exit(EXIT_FAILURE);
        }
        qDebug()<<"Child Memory attached at "<<shm<<endl;
        pShmNotify = (struct shmNotify*)shm;
        pShmNotify->notifyFlag = 0;
        /* 调用iozone主函数 */
        iozoneMain(p, argc, argv);
        /* 一轮测试结束 */
        waitFlag->acquire();/* 若父进程等待 */
        pShmNotify->notifyFlag = -1;/* 结束标志 */
        callFlag->release();/* 子进程已发出通知 */

        /* 把共享内存从当前进程中分离 */
        if(shmdt(pShmNotify) == -1)
        {
            qDebug()<<"shmdt failed"<<endl;
            _exit(EXIT_FAILURE);
        }
        _exit(0);
        /*-----子进程结束-----*/
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
        qDebug()<<"Father Memory attached at "<<shm<<endl;
        pShmNotify = (struct shmNotify*)shm;

        /* 等待处理子进程的通知直到子进程发出结束标志 */
        while(1)
        {
            callFlag->acquire();/* 若子进程已发出通知 */
            if(pShmNotify->notifyFlag == 1)
            {   /* 若子进程通知传数据,父进程创建事件对象传给主窗口 */
                notifyGUI(pShmNotify->type, pShmNotify->kb, pShmNotify->reclen, pShmNotify->speed);
                pShmNotify->notifyFlag = 0;/* 标志位置为已接收 */
                waitFlag->release();/* 父进程等待 */
                continue;
            }
            else if(pShmNotify->notifyFlag == -1)
            {   /* 若子进程通知一轮测试已结束 */
                pShmNotify->notifyFlag = 0;/* 将标志位置为已接收 */
                waitFlag->release();/* 父进程等待 */
                break;
            }
        }

        waitpid(pid, NULL, 0);/* 等待子进程 */

        /* 把共享内存从当前进程分离 */
        if(shmdt(pShmNotify) == -1)
        {
            qDebug()<<"shmdt failed"<<endl;
            exit(EXIT_FAILURE);
        }

        /* 释放对象 */
        delete callFlag;
        delete waitFlag;

        /* 判断测试轮数是否达到要求,没有达到的话再fork一次 */
        if ( i < param->iTestTimes )
        {   /* 下次fork之前恢复初始状态 否则会出错 */
            callFlag = new QSystemSemaphore("notify", 0, QSystemSemaphore::Create);
            waitFlag = new QSystemSemaphore("wait", 1, QSystemSemaphore::Create);
            goto goFork;
        }

        /* 判断是否需要取消挂载 */
        if ( param->bFlagMnt == true )
        {   /* 取消挂载 */
            qsCommand = "umount";
            args.clear();
            args.append(param->qsMntDir);
            QProcess::execute(qsCommand, args);
        }
        if ( param->bFlagShell == true )
        {   /* 执行shell 以"sh ./shell fstype mount"格式 */
            qsCommand = "sh";
            args.clear();
            args << param->qsShellDir << param->qsFsType << "umount";
            QProcess::execute( qsCommand, args );
        }

        /* 测试结束 */
        //*(param->pbFlagRun) = false; 在iozonewidget中修改

        /* 释放内存 */
        for ( i = 0; i < DEFAULT_ARGV; ++i)
        {
                delete argv[i];
        }
        delete this->param;/* 在点击start按钮时getparam()中new的 */
        this->quit();
    }
}



