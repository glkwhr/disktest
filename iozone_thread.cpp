#include "iozone_thread.h"
#include "paintedwidget.h"


extern "C" {
    int iozone_main(int argc, char *argv[32]);

}

#include "iozone_paramwidget.h"

#include <cstdio>

void iozoneThread::run()
{
    int argc = 14;
    char *argv[32];
    for (int i = 0; i < 32; ++i)
    {
        argv[i] = new char[32];
    }

    //   ./iozone -a -s 16m -i 0 -i 1 -i 2 -f tmpfile -Rb res.xls


    int i = -1;
    strcpy(argv[++i], "iozone");
    strcpy(argv[++i], "-a");
    strcpy(argv[++i], "-s");

    sprintf(argv[++i], "%dm", param->file_size);
    //strcpy(argv[++i], "16m");

    if (param->write_checked)
    {
        strcpy(argv[++i], "-i");
        strcpy(argv[++i], "0");
    }
    if (param->read_checked)
    {
        strcpy(argv[++i], "-i");
        strcpy(argv[++i], "1");
    }
    if (param->random_checked)
    {
        strcpy(argv[++i], "-i");
        strcpy(argv[++i], "2");
    }

    strcpy(argv[++i], "-f");

    strcpy(argv[++i], param->file_posi.toStdString().c_str() );
    //strcpy(argv[++i], "tmpfile");


    strcpy(argv[++i], "-Rb");
    strcpy(argv[++i], "res.xls");

    delete param;


    iozone_main(argc, argv);


    for (int i = 0; i < 32; ++i)
    {
        delete[] argv[i];
    }
}



