/*************************************************************************
	> File Name: myft.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Thu 08 Sep 2016 04:27:52 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include "myft.h"

/* judge file type and count */
int myfunc(const char *pathname, const struct stat *statptr, int type)
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
                    printf("for S_IFDIR for %s\n", pathname);
            }
            break;

        /* is directory */
        case FT_D:
            ndir++;
            break;

        /* is directory, but unreadable */
        case FT_DNR:
            printf("%s 目录不可读\n", pathname);
            break;
        case FT_NS:
            printf("%s stat出错\n", pathname);
            break;
        default:
            printf("%d类型不可识别，路径名师 %s\n", type, pathname);
    }

    return 0;
}

/* deal with pathname */
int myft(Myfunc *func)
{

    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;

    /* get stat info of file */
    if (lstat(fullpath, &statbuf) < 0)  //lstat and stat is different in softlink
    {
        func(fullpath,&statbuf,FT_NS);
        goto end;
    }

    /* not a directory */
    if (S_ISDIR(statbuf.st_mode) == 0)
    {
        func(fullpath,&statbuf,FT_F);
        goto end;
    }

    if (func(fullpath,&statbuf,FT_D) != 0) // is not a directory
        return -1;
    else // is a directory
    {
        /* realloc, 1 for '/' */
        if ((fullpath = realloc(fullpath,(pathlen+NAME_MAX+1)*sizeof(char))) == NULL)
            printf("realloc failed!\n");

        /* add '/' in the end if pathname without '/' in the end */
        if (fullpath[pathlen-1] != '/')
        {
            fullpath[pathlen] = '/';
            pathlen++;
        }

        /* traverse every directories */
        if ((dp = opendir(fullpath)) == NULL)
        {
            func(fullpath,&statbuf,FT_DNR);
            goto end;
        }

        while ((dirp = readdir(dp)) != NULL)
        {
            if (strcmp(dirp->d_name,".") == 0 || strcmp(dirp->d_name,"..")==0)
                continue; // avoid '.' and '..'

            strcpy(&fullpath[pathlen],dirp->d_name); // add directory name after '/'

            /* Recursive */
            if (myft(func) != 0)
                break;
        }
    }

    /* close directory */
    if (closedir(dp) < 0)
        printf("can't close directory %s",fullpath);

end:
    return 0;
}


/* malloc for pathname */
void dopath(const char *pathname)
{
    fullpath = NULL;
    pathlen = strlen(pathname);

    /* calloc for pathname */
    fullpath = calloc(pathlen,sizeof(char));
    if (fullpath == NULL)
    {
        fprintf(stderr,"Error: calloc error\n");
        exit(EXIT_FAILURE);
    }

    strcpy(fullpath,pathname);
}

/* print file type result */
void print_result()
{
    printf("file type        amounts  percent\n");
    printf("regular files    %7ld  %5.2f%%\n", nreg, (nreg*100.0/ntot));
    printf("directories      %7ld  %5.2f%%\n", ndir, (ndir*100.0/ntot));
    printf("block special    %7ld  %5.2f%%\n", nblk, (nblk*100.0/ntot));
    printf("char special     %7ld  %5.2f%%\n", nchr, (nchr*100.0/ntot));
    printf("FIFOs            %7ld  %5.2f%%\n", nfifo, (nfifo*100.0/ntot));
    printf("symbolic links   %7ld  %5.2f%%\n", nslink, (nslink*100.0/ntot));
    printf("sockets          %7ld  %5.2f%%\n", nsock, (nsock*100.0/ntot));
    printf("total            %7ld\n", ntot);
}

/* clear up before over */
void clear_up()
{
    free(fullpath);
    fullpath = NULL;
}