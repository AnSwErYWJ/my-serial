/*************************************************************************
	> File Name: main.c
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Mon 25 Jul 2016 11:03:20 AM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<getopt.h>
#include<stdlib.h>
#include<errno.h>
#include"mycp.h"

/* opt flag*/
bool opt_r = false;
bool opt_l = false;
bool opt_s = false;

int main(int argc,char *const argv[])
{
    char c;

    while((c = getopt(argc,argv,"Rrls")) != -1)
    {
        switch(c)
        {
            /*recurrence*/
            case 'R':
            case 'r':
                opt_r = true;
                break;
            /*hard link*/
            case 'l':
                opt_l = true;
                break;
            /*soft link*/
            case 's':
                opt_s = true;
                break;
            default:
                fprintf(stderr,"Usage: Please input correct option[R|r|l|s]\n");
                return EXIT_FAILURE;
        }
    }

    /*check if src and dest exists*/
    if (optind + 1 + 1 != argc)
    {
        fprintf(stderr,"Usage: ./mycp -[R|r|l|s] src dest\n");
        return EXIT_FAILURE;
    }
   
    char *src = NULL;
    char *dest = NULL;

    src = argv[optind];
    dest = argv[optind+1];

    if (opt_l)
    {
        if(isdir(src))
        {
            fprintf(stderr,"Error: dir can not create hard link\n");
            return EXIT_FAILURE;
        }

        if ((link(src,dest)) == 0)
            return EXIT_SUCCESS;
        else
        {
            perror("Error hard link");
            return EXIT_FAILURE;
        }

    }

    if (opt_s)
    {
        if(isdir(src))
        {
            fprintf(stderr,"Error: dir can not create soft link\n");
            return EXIT_FAILURE;
        }

        if ((symlink(src,dest)) == 0)
            return EXIT_SUCCESS;
        else
        {
            perror("Error soft link");
            return EXIT_FAILURE;
        }
        
    }

    if (!isdir(src))
    {
        if ((copyF2F(src,dest)) == 0)
            return EXIT_SUCCESS;
        else
        {
            fprintf(stderr,"Error:copy file failed\n");
            return EXIT_FAILURE;
        }
    }
    else if (isdir(src))
    {
        if (!isdir(dest))
        {
            fprintf(stderr,"Error:copy dir to file failed\n");
            return EXIT_FAILURE;
        }
        else if (isdir(dest) && opt_r)
        {
            if (copyD2D(src,dest) != 0)
            {
                fprintf(stderr,"Error:copy dir failed\n");
                return EXIT_FAILURE;
            }
            else
                return EXIT_SUCCESS;
        }
        else
        {
            fprintf(stderr,"Error:copy dir need option -r\n");
            return EXIT_FAILURE;
        }
    }
    else
    {  
        fprintf(stderr,"Error:operation illegal\n");
        return EXIT_FAILURE;
        
    }
    return EXIT_SUCCESS;
}
