/*************************************************************************
	> File Name: main.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Mon 19 Sep 2016 10:59:18 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "myshell.h"

int main(int argc,char *argv[])
{
    /* check argvs */
    if (argc != 1)
    {
        fprintf(stderr,"Usage: ./myshell\n");
        exit(EXIT_FAILURE);
    }

    /* create new shell in current dir */
    if (chdir(".") != 0)
    {
        fprintf(stderr,"change dir failed\n");
        exit(EXIT_FAILURE);
    }

    int iter = 0;
   
    /* save command(s) */
    char *command = (char *)calloc(MAX+1,sizeof(char));
    if (command ==NULL)
    {
        fprintf(stderr,"calloc failed\n");
        exit(EXIT_FAILURE);    
    }

    while (1)
    {
        iter = 0;


        /* capture signal */
        signal(SIGINT, sig_handle); // SIGINT = 2, Ctrl-C
        signal(SIGQUIT, sig_handle); /* SIGQUIT = 3, Ctrl-\ */
        signal(SIGCHLD, sig_handle); // SIGCHLD = 17, child process generate
        signal(SIGTSTP, sig_handle); // SIGTSTP = 20, Ctrl-D

        /* print prompt */
        print_prompt();

        /* scan command ,save to command */
        scan_command(command);

        /* splite commands by ';' ,then save to all */
        parse_semicolon(command);

        /* execute commands */
        while (all[iter] != NULL)
            execute(all[iter++]);
    }
    
    return 0;
}
