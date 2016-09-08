/*************************************************************************
	> File Name: main.c
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Tue 02 Aug 2016 03:23:37 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "mywho.h"

bool opt_H = false;
bool opt_q = false;
bool opt_b = false;

int main(int argc,char *const argv[])
{
    char c;

    /* check argv */
    while ((c = getopt(argc,argv,"Hqb")) != -1)
    {
        switch (c)
        {
            case 'H':
                opt_H = true;
                break;
            case 'q':
                opt_q = true;
                break;
            case 'b':
                opt_b = true;
                break;
            default:
                fprintf(stderr,"Usage: Please input correct option[H|q|b]\n");
                return EXIT_FAILURE;
        }
    }

    if (argc >= optind + 1)
    {
        fprintf(stderr,"Usage:./mywho [H|q|b]\n");
        return EXIT_FAILURE;
    }

    if (!mywho())
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}
