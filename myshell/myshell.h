/*************************************************************************
	> File Name: myshell.h
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Mon 19 Sep 2016 03:55:18 PM CST
 ************************************************************************/

#ifndef _MYSHELL_H
#define _MYSHELL_H

#define MAX 1024 // max number of commands
#define MAX_COMM 100 // max length of command

char cwd[MAX]; // current path
char *all[MAX]; // commands

/* background processes*/
typedef struct proc{
    pid_t pid;
    int status;
    char *arg[MAX_COMM];
    struct proc *next;
}proc;
proc *start;


void print_prompt(); /* print prompt */
void sig_handle(int sig); /* signal processing function */
void scan_command(char *command); /* scan command */
void parse_semicolon(char *command); /* splite commands by ';' ,then save to all */

#endif
