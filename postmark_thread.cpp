#include "postmark_thread.h"

extern "C" {
    int postmark_main(int argc, char *argv[32]);

}

#include "postmark_paramwidget.h"
#include <cstdio>


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


    sprintf(argv[++i], "set location %s", param->file_posi.toStdString().c_str() );

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

    //开始postmark的测试
    postmark_main(argc, argv);


    for (int i = 0; i < 32; ++i)
    {
        delete[] argv[i];
    }
}
