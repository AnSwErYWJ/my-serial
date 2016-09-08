/*************************************************************************
	> File Name: gethostbyname.c
	> Author: AnSwEr 
	> Mail: 
	> Created Time: 2015年07月16日 星期四 21时34分06秒
 ************************************************************************/

/*
 * 根据域名获取ip地址
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netdb.h>

int main(int argc,const char *argv[])
{
    int i;
    struct hostent *host;
    char str[16];

    if(argc < 2)
    {
        printf("Usage: %s <hostname>\n",argv[0]);
        exit(1);
    }

    host=gethostbyname(argv[1]);

    for(i = 0;host->h_addr_list[i];i++)
    {
        if(inet_ntop(AF_INET,host->h_addr_list[i],str,sizeof(str)) != NULL)
        {
            printf("IP addr %d : %s \n",i+1,str);
        }
    }

    return 0;
}

