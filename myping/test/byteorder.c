/*************************************************************************
	> File Name: byteorder.c
	> Author: 
	> Mail: 
	> Created Time: 2015年07月16日 星期四 20时11分34秒
 ************************************************************************/

/*
 * 实现主机字节序网络字节序的转换
 */

#include<stdio.h>
#include<arpa/inet.h>

int main(void)
{
    unsigned short hosts = 0x1234;//unsigned short 2个字节，16位
    unsigned short nets;
    unsigned long hostl = 0x12345678;//unsigned long 4个字节，32位
    unsigned long netl;

    nets = htons(hosts);
    netl = htonl(hostl);

    printf("Host ordered short : %#x \n",hosts);
    printf("Network ordered short : %#x \n",nets);
    printf("Host ordered long : %#lx \n",hostl);
    printf("Network ordered long : %#lx \n",netl);

    return 0;
}
