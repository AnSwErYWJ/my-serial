/*************************************************************************
	> File Name: myshell.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Mon 19 Sep 2016 03:55:25 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/utsname.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "myshell.h"

#define MAX_COMM 100 // max length of one command

static int current_out = 4;
static int current_in = 5;

static char cwd[MAX]; // current path

/* background processes*/
typedef struct proc{
    pid_t pid;
    int status;
    char *arg[MAX_COMM];
    struct proc *next;
}proc;
static proc *start;

/* splite command by ' '*/
static void *parse(char *command, int times)
{
    char *comm;

    if(times ==0)
        comm = strtok(command, " ");
    else
        comm = strtok(NULL, " ");
    return comm;
}

/* built-in cd */
static void cd(char *arg)
{
    /* default dir "~" */
    if(arg == NULL)
        fprintf(stderr,"Error: insufficient arguments\n");
    else
    {
        if(chdir(arg) != 0)
        {
            fprintf(stderr,"Error: wrong path for cd \n");
        }
    }
}

/*
 * handle proc list
 * 0: add
 * 1: remove
 * 2: print
 */
void bg_struct_handle(pid_t pid, char *arg[], int type)
{
    proc *iterate, *new;
    proc *preiter = NULL;
    
    int i = 1, a = 0;
    
    switch (type)
    {
        /* add */
        case 0:
            if(start == NULL)
            {
                start = (proc *)malloc(sizeof(proc));
                start -> pid = pid;
                start -> status = 1;
                start -> next = NULL;
                int i = 0;
                while(arg[i] != NULL)
                {
                    start -> arg[i] = malloc(MAX_COMM * sizeof(char));
                    strcpy(start -> arg[i], arg[i]);
                    i += 1;
                }
                start -> arg[i] = NULL;
            }
            else
            {
                new = (proc *)malloc(sizeof(proc));
                new -> pid = pid;
                new -> status = 1;
                new -> next = NULL;
                int i = 0;
                while(arg[i] != NULL)
                {
                    new -> arg[i] = malloc(MAX_COMM * sizeof(char));
                    strcpy(new -> arg[i], arg[i]);
                    i += 1;
                }
                new -> arg[i] = NULL;
                iterate = start ;
                while(iterate -> next != NULL)
                    iterate = iterate -> next;
                iterate -> next = new;
            }
            break;

        /* remove */
        case 1:
            iterate = start;
            while(iterate != NULL && iterate -> pid != pid )
            {
                preiter = iterate;
                iterate = iterate -> next;
            }
            if(iterate == NULL)
            {
                fprintf(stderr,"Error:No Such Pid !\n");
                return ;
            }
            else if(iterate -> pid == pid)
            {
                if(preiter == NULL)
                {
                    start = iterate -> next;
                    free(iterate);
                }
                else
                {
                    preiter -> next = iterate -> next;
                    free(iterate);
                }
            }
            break;

        /* print */
        case 2:
            iterate = start;
            if (iterate == NULL)
            {
                printf("No Background jobs\n");
                return ;
            }
            while(iterate != NULL)
            {
                a = 0;
                setbuf(stdout, NULL);
                printf("[%d] ",i);
                while(iterate -> arg[a] != NULL)
                {
                    printf("%s ", iterate -> arg[a]);
                    a += 1;
                }
                printf("[%d]\n", iterate -> pid);
                i += 1;
                iterate = iterate -> next;
            }
            break;

        default:
            fprintf(stderr,"Error: handle proc list!\n");
    }

    return ;
}


/* handle background process proc */
void bg_signal_handle()
{
    pid_t pid = waitpid(-1, NULL, WNOHANG); // -1 : same sa wait

    proc *iterate = start;

    while(iterate != NULL)
    {
        if(iterate->pid == getpid())
        {
            bg_struct_handle(pid, NULL, 1); // remove child process(over) info rome proc
        }
    }
}

/*
 * real execute command
 * 0:foreground
 * 1:background
 */
static void bf_exec(char *arg[], int type)
{
    pid_t pid;

    /* foreground */
    if(type == 0)
    {
        /* fork failed */
        if((pid = fork()) < 0)
        {
            fprintf(stderr,"ERROR: forking child process failed\n");
            return ;
        }

        /* child process */
        else if(pid == 0)
        {
            signal(SIGTSTP, SIG_DFL); // SIGTSTP : Ctrl-Z SIG_DFL: stop
            execvp(arg[0], arg);
        }

        /* father process */
        else
        {
            signal(SIGTSTP, SIG_DFL);  // SIGTSTP : Ctrl-Z SIG_DFL: stop

            wait(NULL);

            /* reset stdout and stdin */
            dup2(current_out, 1);
            dup2(current_in, 0);

            return;
        }
    }

    /* background */
    else
    {
        signal(SIGCHLD, bg_signal_handle);

        /* fork failed */
        if((pid = fork()) < 0)
        {
            fprintf(stderr,"ERROR: forking child process failed\n");
            return ;
        }

        /* child process */
        else if(pid == 0)
            execvp(arg[0], arg);

        /* father process */
        else
        {
            bg_struct_handle(pid, arg, 0);

            /* reset stdout and stdin */
            dup2(current_out, 1);
            dup2(current_in, 0);

            return ;
        }
    }
}

/* 
 * 0 : '>' 
 * 1 : '>>' 
 */
static void file_out(char *arg[], char *out_file, int type)
{
    /* copy stdout */
    current_out = dup(1);

    int f;

    /* > */
    if(type == 0)
    {
        f = open(out_file, O_WRONLY | O_CREAT, 0777);
        dup2(f, 1); //copy 1 to f
        close(f);
        bf_exec(arg, 0);
    }
    /* >> */
    else
    {
        f = open(out_file, O_WRONLY | O_CREAT | O_APPEND , 0777);
        dup2(f, 1); //copy 1 to f
        close(f);
        bf_exec(arg, 0);
    }
}

/* 
 * 0 : '<'
 * 1 : 'xx < xx > xx'
 * 2 : 'xx < xx >> xx'
 */
static void file_in(char *arg[], char *in_file, char *out_file, int type)
{
    int in = open(in_file, O_RDONLY);

    /* copy 0 to in */
    current_in = dup(0);
    dup2(in, 0);
    close(in);

    /* '<' */
    if(type == 0)
        bf_exec(arg, 0);

    /* `xx < xx > xx` */
    else if(type == 1)
        file_out(arg, out_file, 0);

   /* `xx < xx >> xx` */
    else
        file_out(arg, out_file, 1);
}

/*################## end internal call #######################################*/

/* print prompt */
void print_prompt()
{
    /* get system info */
    struct utsname uname_ptr;
    if (uname(&uname_ptr) != 0)
        fprintf(stderr,"Error: get system info error\n");

    /* get current path name */
    getcwd(cwd,sizeof(cwd));

    setbuf(stdout,NULL);

    /* name in network,system name */
    printf("<%s@%s:%s>",uname_ptr.nodename,uname_ptr.sysname,cwd);
}

/* signal processing function */
void sig_handle(int sig)
{
    /* SIGINT = 2 */
    if (sig == 2)
    {
        printf("\nInstead of Ctrl-C ,Please type quit\n");
        print_prompt(); //print prompt
    }
    /* SIGQUIT = 3 */
    else if (sig == 3)
    {
        printf("\nType exit to quit\n");
        print_prompt(); //print prompt
    }

    signal(sig,sig_handle); // SIGCHLD = 17
}

/* scan command */
void scan_command(char *command)
{
    int bytes_read;
    size_t nbytes = MAX;

    /* read commands from stdin */
    bytes_read = getline(&command,&nbytes,stdin);

    /* '\0' instead of '\n' */
    bytes_read -= 1;
    command[bytes_read] = '\0';
}

/* splite commands by ';' ,then save to all */
void parse_semicolon(char *command)
{
    int i ;

    /* calloc for every commands */
    for (i=0; i < MAX; i++)
    {
        all[i] = (char *)calloc(MAX_COMM,sizeof(char));
        if (all[i] ==NULL)
            fprintf(stderr,"calloc failed\n");
    }

    /* splite commands by ';' */
    i = 0;
    all[i] = strtok(command, ";");
    while(1)
    {
        i++;
        all[i] = strtok(NULL, ";");
        if(all[i] == NULL)
            break;
    }
}

/* execute command */
void execute(char *command)
{
    char *arg[MAX_COMM];
    char *try;
    int t = 1;

    /* build-in commands*/
    arg[0] = parse(command, 0); //get first character of command,such as “ls”
    arg[t] = NULL;

    /* command cd */
    if (strcmp(arg[0], "cd") == 0)
    {
        try = parse(command, 1);
        cd(try);
        return;
    }
    /* command quit */
    if (strcmp(arg[0], "quit") == 0)
        exit(EXIT_SUCCESS);

    /* no-build-in command */
    while (1)
    {
        /* get argument */
        try = parse(command, 1);

        /* no arguments for commands */
        if (try == NULL)
            break;

        /* second arguments is '>' */
        else if (strcmp(try, ">") == 0)
        {
            try = parse(command, 1); // get argument
            file_out(arg, try, 0);
            return;
        }

        /* second arguments is '>>' */
        else if (strcmp(try, ">>") == 0)
        {
            try = parse(command, 1); // get argument
            file_out(arg, try, 1);
            return;
        }

        /* second arguments is '<' */
        else if (strcmp(try, "<") == 0)
        {
            try = parse(command, 1); // get argument

            char *out_file = parse(command, 1);  // get argument

            if (out_file != NULL)
            {
                /* xx < xx > xx */
                if (strcmp(out_file, ">") == 0)
                {
                    out_file = parse(command, 1); // get argument

                    if (out_file == NULL)
                    {
                        fprintf(stderr,"Syntax error !\n");
                        return;
                    }
                    else
                        file_in(arg, try, out_file, 1);
                }
                /* xx < xx >> xx */
                else if (strcmp(out_file, ">>") == 0)
                {
                    out_file = parse(command, 1); // get argument

                    if (out_file == NULL)
                    {
                        fprintf(stderr,"Syntax error !\n");
                        return;
                    }
                    else
                        file_in(arg, try, out_file, 2);
                }
            }
            else
            {
                file_in(arg, try, NULL, 0);
            }
        }

        /* '&' background */
        else if (strcmp(try, "&") == 0)
        {
            bf_exec(arg, 1);
            return;
        }

        else
        {
            arg[t] = try;
            arg[++t] = NULL;
        }
    }

    bf_exec(arg, 0); 
}
