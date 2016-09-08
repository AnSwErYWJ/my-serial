/*************************************************************************
	> File Name: mytouch.c
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Thu 04 Aug 2016 01:56:58 PM CST
 ************************************************************************/

#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>

#include"mytouch.h"

bool mytouch(const char *file)
{
    bool ok;
    int fd;

    if(!no_create)
    {
        fd = open(file,O_CREAT|O_WRONLY,MODE_RW_UGO);
        if(fd == -1)
            fprintf(stderr,"Error:open %s failed.\n",file);
    }

    if(change_times != (CH_ATIME | CH_MTIME))
    {
        if(change_times == CH_MTIME)
            newtime[0].tv_nsec = UTIME_OMIT;
        else
            newtime[1].tv_nsec = UTIME_OMIT;
    }

    /* AT_FDCWD : ./ */
    ok = (utimensat(AT_FDCWD,file,newtime,0) == 0);

    return ok;
}
