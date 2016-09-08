/*************************************************************************
	> File Name: main.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Thu 08 Sep 2016 11:26:20 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

/*
 * nreg：普通文件的数量； 
 * ndir：目录文件的数量； 
 * nblk：块特殊文件的数量； 
 * nchr：字符特殊文件的数量； 
 * nfifo：管道特殊文件的数量； 
 * nslink：符号链接文件的数量； 
 * nsock：套接字文件数量； 
 * ntot：总共文件数量；
 */
static long nreg, ndir, nblk, nfifo, nchr, nslink, nsock, ntot;

static int myft(char *pathname,Myfunc *func)

int main(int argc,const char *argv[])
{
    int result = -1;

    /* check arguments */
    if (argc != 2)
    {
        printf("Usage:\n");
        exit(EXIT_FAILURE);
    }

    result = myft(argv[1],myfunc);
    
    /* total numbers */
    ntot = nreg + ndir + nblk + nfifo + nchr + nslink + nsock + ntot;

    /* avoid divisor equal 0 */
    ntot = ntot ? ntot : 1;

    printf("file type        amounts  percent\n");
    printf("regular files    %7ld  %5.2f%%\n", nreg, (nreg*100.0/ntot));
    printf("directories      %7ld  %5.2f%%\n", ndir, (ndir*100.0/ntot));
    printf("block special    %7ld  %5.2f%%\n", nblk, (nblk*100.0/ntot));
    printf("char special     %7ld  %5.2f%%\n", nchr, (nchr*100.0/ntot));
    printf("FIFOs            %7ld  %5.2f%%\n", nfifo, (nfifo*100.0/ntot));
    printf("symbolic links   %7ld  %5.2f%%\n", nslink, (nslink*100.0/ntot));
    printf("sockets          %7ld  %5.2f%%\n", nsock, (nsock*100.0/ntot));
    printf("total            %7ld\n", ntot);
    
    return result;
}
