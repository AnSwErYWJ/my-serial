/*************************************************************************
	> File Name: mytouch.h
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Thu 04 Aug 2016 01:57:04 PM CST
 ************************************************************************/

#ifndef _MYTOUCH_H
#define _MYTOUCH_H

typedef enum _bool{
    false,
    true,
}bool;

int change_times;
bool no_create;
struct timespec newtime[2];

#define CH_ATIME 1
#define CH_MTIME 2

#define MODE_RW_UGO (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

bool mytouch(const char *file);

#endif
