/*************************************************************************
	> File Name: mycp.c
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Mon 25 Jul 2016 05:24:38 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<dirent.h>
#include"mycp.h"

/* limit of file name */
#define MAX 256

#define BUF_SIZE 4096

bool isdir(const char *filename)
{
    struct stat fileinfo;

    /* get file infomation */
    if(stat(filename,&fileinfo) == 0)
    {
        if(S_ISDIR(fileinfo.st_mode))
            return true;
    }
    return false;   
}

static char *_reverse(char *str)
{
    int i;
    int len = strlen(str);
    char tmp;

    for(i=0;i<len/2;i++)
    {
        tmp = *(str+i);
        *(str+i) = *(str+len-1-i);
        *(str+len-1-i) = tmp;
    }
    
    return str;
}

ssize_t safe_read(int fd,void *vptr,size_t n)
{
    size_t nleft;
    ssize_t nread = 0;
    char *ptr;

    ptr = vptr;
    nleft = n;

    while(nleft > 0)
    {
        if((nread = read(fd,ptr,nleft)) < 0)
        {
            if(errno == EINTR) //read again
                nread = 0;
            else
            {    
                fprintf(stderr,"Error: read file failed!\n");
                return -1;
            }
        }
        else if(nread == 0) //EOF
            break;
        nleft -= nread;
        ptr += nread;
    }
    return(n - nleft);
}

ssize_t safe_write(int fd,const void *vptr,size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = vptr;
    nleft = n;

    while(nleft > 0)
    {
        if((nwritten = write(fd,ptr,nleft)) <= 0)
        {
            if(nwritten < 0 && errno == EINTR)// write again
                nwritten = 0;
            else
            {
                fprintf(stderr,"Error: write file failed!\n");
                return -1;
            }
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

/* src is a file */
int copyF2F(const char *src,char *dest)
{
    /* if dest is dir,create a file in dest */
    if(isdir(dest))
    {
        char tmp[MAX];
        memset(tmp,'\0',sizeof(tmp));
        
        int src_len = strlen(src);
        int dest_len = strlen(dest);
        
        int i = 0;
        char c;

        /* get filename from path */
        while((c = src[--src_len]) != '/')
            tmp[i++] = c;
       
        /* if not '/',add it*/
        //if(dest[dest_len-1] != '/')
        //    strcat(dest,"/");

        strcat(dest,_reverse(tmp));
    }

    int src_fd,dest_fd;

    if((src_fd = open(src,O_RDONLY)) == -1)
    {
        fprintf(stderr,"open %s",src);
        perror(" ");
        return 1;
    }

    /* open dest only write,create with 664*/
    if((dest_fd = open(dest,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) == -1)
    {
        fprintf(stderr,"open %s",dest);
        perror(" ");
        return 1;
    }

    char buf[BUF_SIZE];
    memset(buf,'\0',sizeof(buf));
    ssize_t size = 0;
    
    while((size = safe_read(src_fd,buf,BUF_SIZE)) > 0)
    {
        if(safe_write(dest_fd,buf,size) != size)
        {
            fprintf(stderr,"Error: write %s  %ld bytes failed!\n",dest,size);
            return 1;
        }
    }

    if(close(src_fd) == -1 || close(dest_fd) == -1)
    {
        fprintf(stderr,"Error: close file failed!\n");
        return 1;
    }

    return 0;
}

int copyD2D(const char *src,const char *dest)
{
    DIR *dp = NULL;
    struct dirent *dirp;
    char tmpdest[MAX];
    char tmpsrc[MAX];

    strcpy(tmpdest,dest);
    strcpy(tmpsrc,src);

    if((dp = opendir(src)) == NULL)
    {    
        fprintf(stderr,"Error: opendir %s failed\n",src);
        return 1;
    }
    else
    {
        while((dirp = readdir(dp)))
        {
            if(!isdir(dirp->d_name))
            {
                strcat(tmpdest,dirp->d_name);
                strcat(tmpsrc,dirp->d_name);
                
                copyF2F(tmpsrc,tmpdest);

                strcpy(tmpdest,dest);
                strcpy(tmpsrc,src);
            }
        }
    closedir(dp);
    return 0;
    }
}
