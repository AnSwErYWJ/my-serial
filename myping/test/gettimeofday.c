/*************************************************************************
	> File Name: gettimeofday.c
	> Author: 
	> Mail: 
	> Created Time: 2015年07月16日 星期四 20时30分44秒
 ************************************************************************/

/*
 * 计算程序运行的时间
 */

#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>

// 计算时间差，单位（微秒）us

int timediff(struct timeval *begin,struct timeval *end)
{
    int n;
    n=(end->tv_sec - begin->tv_sec)*1000 + (end->tv_usec - begin->tv_usec);
    return n;
}

int main(void)
{
    struct timeval begin,end;
    gettimeofday(&begin,0);

    printf("do something here\n");
    sleep(1);

    gettimeofday(&end,0);
    printf("running time : %dus\n",timediff(&begin,&end));

    return 0;
}
