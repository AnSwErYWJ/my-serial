/*************************************************************************
	> File Name: main.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Mon 19 Sep 2016 10:59:18 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

/* signal processing function */
void sig_handle()
{}

/* print prompt*/
void print_prompt()
{}

/* scan command */
void scan_command(char *command)
{}

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
    char *command = (char *)calloc()
    while (0)
    {
        iter = 0;

        /* capture signal */
        signal(SIGINT, sig_handle); // SIGINT = 2, Ctrl-C
        signal(SIGQUIT, sig_handle); /* SIGQUIT = 3, Ctrl-\ */
        signal(SIGCHLD, sig_handle); // SIGCHLD = 17, child process generate
        signal(SIGTSTP, sig_handle); // SIGTSTP = 20, Ctrl-D

        /* print prompt*/
        print_prompt();

        /* scan command */
        scan_command(command);
    }

    return 0;
}
