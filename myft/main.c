/*************************************************************************
	> File Name: main.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Thu 08 Sep 2016 11:26:20 AM CST
 ************************************************************************/

#include <stdio.h>
#include "myft.h"

int main(int argc,const char *argv[])
{
    /* check arguments */
    if (argc != 2)
    {
        fprintf(stderr,"Usage: ./myft <pathname>\n");
        return -1;
    }

    /* malloc for pathname */
    dopath(argv[1]);

    /* deal with pathname */
    int ret = -1;
    ret = myft(myfunc);
    
    /* total numbers */
    ntot = nreg + ndir + nblk + nfifo + nchr + nslink + nsock;
    ntot = ntot ? ntot : 1; // avoid divisor equal 0

    /* print file type result */
    print_result();

    /* clear up before over */
    clear_up();

    return ret;
}
