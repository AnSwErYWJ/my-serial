/*************************************************************************
	> File Name: pton.c
	> Author:AnSwEr 
	> Mail: 
	> Created Time: 2015年07月16日 星期四 21时19分55秒
 ************************************************************************/

/*
 * 32位大端序整型格式ip地址与点分十进制格式之间的转换
 */

#include<stdio.h>
#include<arpa/inet.h>

int main(void)
{
    struct sockaddr_in addr1,addr2;
    const char *str1 = "127.0.0.1";
    const char *str2 = "1.2.3.4";
    char str3[16];
    char str4[16];

    if(inet_pton(AF_INET,str1,&addr1.sin_addr))
    {
        printf("%s -> %#x \n",str1,addr1.sin_addr.s_addr);
    }
    if(inet_pton(AF_INET,str2,&addr2.sin_addr))
    {
        printf("%s -> %#x \n",str2,addr2.sin_addr.s_addr);
    }

    if(inet_ntop(AF_INET,&addr1.sin_addr.s_addr,str3,sizeof(str3)) != NULL)
    {
        printf("%#x -> %s \n",addr1.sin_addr.s_addr,str3);
    }
    if(inet_ntop(AF_INET,&addr2.sin_addr.s_addr,str4,sizeof(str4)) != NULL)
    {
        printf("%#x -> %s \n",addr2.sin_addr.s_addr,str4);
    }

    return 0;
}

