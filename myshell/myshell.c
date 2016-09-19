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

#include "myshell.h"


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
        arg = "~";
    else
    {
        if(chdir(arg) != 0)
        {
            fprintf(stderr,"Error: wrong path for cd \n");
        }
    }
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
    setbuf(stdout,NULL); // set buffer false

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
        printf("\nType quit to exit\n");
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

/* 执行单条命令行语句 */
void execute(char *command)
{
    char *arg[MAX_COMM];
    char *try;
    arg[0] = parse(command, 0);        //获得命令名称的字符串指针，如“ls”
    int t = 1;
    arg[t] = NULL;

    if (strcmp(arg[0], "cd") == 0)     // 处理内嵌命令“cd”的情况
    {
        try = parse(command, 1);
        cd(try);
        return ;
    }

    if (strcmp(arg[0], "exit") == 0) // 为了方便用户推出shell
        exit(0);

    // 循环检测剩下的命令参数，即检测是否：重定向？管道？后台执行？普通命令参数？
    while (1)
    {
        try = parse(command, 1);
        if (try == NULL)
            break;

        else if (strcmp(try, ">") == 0)  // 重定向到一个文件的情况
        {
            try = parse(command, 1);   // 得到输出文件名
            file_out(arg, try, 0);        // 参数 0 代表覆盖的方式重定向
            return;
        }

        else if (strcmp(try, ">>") == 0)   // 追加重定向到一个文件
        {
            try = parse(command, 1);
            file_out(arg, try, 1);        // 参数 1 代表追加的方式重定向
            return;
        }

        else if (strcmp(try, "<") == 0)    // 标准输入重定向
        {
            try = parse(command, 1);      // 输入重定向的输入文件
            char *out_file = parse(command, 1);    // 输出重定向的输出文件
            if (out_file != NULL)
            {
                if (strcmp(out_file, ">") == 0)   // 输入输出文件给定
                {
                    out_file = parse(command, 1);
                    if (out_file == NULL)
                    {
                        printf("Syntax error !!\n");
                        return;
                    }
                    else
                        file_in(arg, try, out_file, 1);     // 参数 1 针对双重定向 < >
                }
                else if (strcmp(out_file, ">>") == 0)
                {
                    out_file = parse(command, 1);
                    if (out_file == NULL)
                    {
                        printf("Syntax error !!\n");
                        return;
                    }
                    else
                        file_in(arg, try, out_file, 2);       // 参数 2 针对双重定向 < >>
                }
            }
            else
            {
                file_in(arg, try, NULL, 0);        // 模式 0 针对单一的输入重定向
            }
        }

        //处理后台进程
        else if (strcmp(try, "&") == 0)   // 后台进程
        {
            bf_exec(arg, 1);     // bf_exec 的第二个参数为 1 代表后台进程
            return;
        }

        else       //try是一个命令参数
        {
            arg[t] = try;
            t += 1;
            arg[t] = NULL;
        }
    }

    bf_exec(arg, 0);     // 参数 0 表示前台运行
    return;
}