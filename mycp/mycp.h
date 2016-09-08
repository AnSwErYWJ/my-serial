/*************************************************************************
	> File Name: mycp.h
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Mon 25 Jul 2016 05:24:31 PM CST
 ************************************************************************/

#ifndef _MYCP_H
#define _MYCP_H

typedef enum bool_ {
      false,
      true,
}bool;

int copyF2F(const char *src,char *dest);

int copyD2D(const char *src,const char *dest);

bool isdir(const char *filename);
#endif
