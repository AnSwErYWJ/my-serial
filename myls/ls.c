/*************************************************************************
	> File Name: ls.c
	> Author:AnSwEr 
	> Mail: 
	> Created Time: Tue 26 Apr 2016 02:44:08 PM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>

void list(char *dir)
{
    DIR *dir_ptr;
    struct dirent *direntp;

    dir_ptr = opendir(dir);
    if(dir_ptr == NULL)
        fprintf(stderr,"Error:open %s failed!\n",dir);
    else
    {
        while((direntp = readdir(dir_ptr)) != NULL)
            printf("%s\n",direntp->d_name);
        closedir(dir_ptr);
    }
}

int main(int argc,char *argv[])
{
    if(argc > 10)
    {
        fprintf(stderr,"Error:your argc=%d out of limit(10)\n",argc);
        return;
    }

    if(argc == 1) // just ls
    {
        list(".");
        return 0;
    }
    
    while(--argc) // list all *argvs
    {
        printf("%s:\n",*++argv);
        list(*argv);
        printf("\n");
    }

    return 0;
}
