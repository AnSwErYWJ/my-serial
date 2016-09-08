/*************************************************************************
	> File Name: main.c
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Thu 04 Aug 2016 01:56:50 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>

#include"mytouch.h"

int main(int argc,char * const argv[])
{
    int c;
    bool ok = true;
    
    change_times = 0;
    no_create = false;

    while((c = getopt(argc,argv,"acm")) != -1)
    {
        switch(c)
        {
            /* change only the access time */
            case 'a':
                change_times |= CH_ATIME;
                break;
            /* do not create any files */
            case 'c':
                no_create = true;
                break;
            /* change only the modification time */
            case 'm':
                change_times |= CH_MTIME;
                break;
            default:
                fprintf(stderr,"fault option\n");
                exit(EXIT_FAILURE);
        }
    }

    /* for -c or no option */
    if(change_times == 0)
        change_times = CH_ATIME | CH_MTIME;

    /* 
     * 0 : access time 1 : modify time 
     * UTIME_OMIT : no change  UTIME_NOW : change to now 
     */
    newtime[0].tv_nsec = UTIME_NOW;
    newtime[1].tv_nsec = UTIME_NOW;

    if(optind + 1 > argc)
    {
        fprintf(stderr,"Usage:./mytouch -[a|c|m] files\n");

    }

    for(;optind < argc;optind++)
        ok &= mytouch(argv[optind]);
    
    exit(ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
