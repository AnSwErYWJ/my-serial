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

int main(int argc,const char *argv[])
{
    if (argc != 2)
    {
        printf("Usage:\n");
        exit(EXIT_FAILURE);
    }


    
}
