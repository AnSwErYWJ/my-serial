/*************************************************************************
	> File Name: main.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Thu 08 Sep 2016 11:26:20 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define FT_F 1 // not directory
#define FT_D 2 // is directory
#define FT_DNR 3 // unreadable
#define FT_NS 4 // no stat

static char *fullpath; // file path name
static size_t pathlen; // file path lenngth

typedef int Myfunc(const char *,const struct stat *,int);
static Myfunc myfunc;

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

/* malloc for pathname*/
static char *path_alloc(size_t *size)
{
    char *p = NULL;
    if (!size)
        return NULL;
    p = malloc(256);
    if (p)
        *size = 256;
    else
        *size = 0;
    
    return p;
}

static int myfunc(const char *pathname, const struct stat *statptr, int type)
{
    switch (type)
    {
        /* not directory*/
        case FT_F:
            switch (statptr->st_mode & S_IFMT)
            {
                case S_IFREG:    
                    nreg++;        
                    break;
                case S_IFBLK:    
                    nblk++;        
                    break;
                case S_IFCHR:    
                    nchr++;        
                    break;
                case S_IFIFO:    
                    nfifo++;    
                    break;
                case S_IFLNK:    
                    nslink++;    
                    break;
                case S_IFSOCK:    
                    nsock++;    
                    break;
                case S_IFDIR:    
                    printf("for S_IFDIR for %s", pathname);
            }
            break;
        
        /* is directory */
        case FT_D:
            ndir++;
            break;

        /* is directory, but unreadable */
        case FT_DNR:
            printf("%s 目录不可读", pathname);
                break;
            case FT_NS:
                printf("%s stat出错", pathname);
                break;
            default:
                printf("%d类型不可识别，路径名师 %s", type, pathname);
    }

    return 0;
}


static int dopath(Myfunc *func)
{
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    int ret, n;
    
    /* get stat info of file */
    if (lstat(fullpath, &statbuf) < 0)
        return (func(fullpath,&statbuf,FT_NS));

    /* not a directory */
    if (S_ISDIR(statbuf.st_mode) == 0)
        return (func(fullpath,&statbuf,FT_F));

    /* is a directory */
    if ((ret = func(fullpath,&statbuf,FT_D)) != 0)
        return ret;

    /* realloc */
    n = strlen(fullpath);
    if (n + NAME_MAX +2 > pathlen)
    {
        pathlen *= 2;
        if ((fullpath = realloc(fullpath,pathlen)) == NULL)
            printf("realloc failed!\n");
    }
    fullpath[n++] = '/';
    fullpath[n] = 0;

    /* do every directories*/
    if ((dp = opendir(fullpath)) == NULL)
        return (func(fullpath,&statbuf,FT_DNR));
    while ((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name,".") == 0 || strcmp(dirp->d_name,"..")==0)
            continue; // avoid '.' and '..'
        strcpy(&fullpath[n],dirp->d_name); // add directory name after '/'
        if ((ret = dopath(func)) != 0)
            break;
    }
    fullpath[n-1] = 0;

    /* close directory */
    if (closedir(dp) < 0)
        printf("can't close directory %s",fullpath);
    return (ret);
}

static int myft(const char *pathname,Myfunc *func)
{
    /* malloc for pathname*/
    fullpath = path_alloc(&pathlen);

    if (pathlen <= strlen(pathname))
    {
        pathlen = strlen(pathname) * 2;
        if ((fullpath = realloc(fullpath,pathlen)) == NULL);
            printf("realloc failed!\n");
    }

    strcpy(fullpath,pathname);

    return (dopath(func));
}

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
