/*************************************************************************
	> File Name: myft.h
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Thu 08 Sep 2016 04:27:48 PM CST
 ************************************************************************/

#ifndef _MYFT_H
#define _MYFT_H

/* flag */
#define FT_F 1 // not directory
#define FT_D 2 // is directory
#define FT_DNR 3 // unreadable
#define FT_NS 4 // no stat

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
long nreg, ndir, nblk, nfifo, nchr, nslink, nsock, ntot;

char *fullpath; // file path name
size_t pathlen; // file path lenngth

typedef int Myfunc(const char *,const struct stat *,int);

Myfunc myfunc;// judge file type and count
int myft(Myfunc *func); // deal with pathname
void dopath(const char *pathname); // malloc for pathname
void print_result(); // print file type result
void clear_up();// clear up before over

#endif
