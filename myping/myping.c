/*************************************************************************
	> File Name: myping.c
	> Author: 
	> Mail: 
	> Created Time: 2015年07月17日 星期五 11时08分25秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "myping.h"

/*
 * DEBUG调试宏
 */
#if defined(DEBUG_PRINT)
#define DEBUG(...)\
    do{\
        fprintf(stderr,"-----DEBUG-----\n");\
        fprintf(stderr,"%s %s\n",__TIME__,__DATE__);\
        fprintf(stderr,"%s:%d:%s():",__FILE__,__LINE__,__func__);\
        fprintf(stderr,__VA_ARGS__);\
    }while(0)
#endif

/*
 * addr 指向需校验数据缓冲区的指针
 * len  需校验数据的总长度（字节单位）
 */
unsigned short checkSum(unsigned short *addr, int len)
{
    unsigned int sum = 0;  
    while(len > 1)
    {
        sum += *addr++;
        len -= 2;
    }

    // 处理剩下的一个字节
    if(len == 1){
        sum += *(unsigned char *)addr;
    }

    // 将32位的高16位与低16位相加
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return (unsigned short) ~sum;
}

/*
 * 返回值单位：ms
 * begin 开始时间戳
 * end   结束时间戳
 */
float timediff(struct timeval *begin, struct timeval *end)
{
    float n;
    // 先计算两个时间点相差多少微秒
    n = ( end->tv_sec - begin->tv_sec ) * 1000000
        + ( end->tv_usec - begin->tv_usec );
    
    // 转化为毫秒返回
    return (n/1000);
}

/*
 * icmp 指向需要封装的ICMP报文结构体的指针
 * sequence 该报文的序号
 */ 
void pack(struct icmp * icmp, int sequence)
{
    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->checksum = 0;
    icmp->id = getpid();
    icmp->sequence = sequence;
    gettimeofday(&icmp->timestamp, 0);
    icmp->checksum = checkSum((unsigned short *)icmp,ICMP_SIZE);
}

/*
 * buf  指向接收到的IP报文缓冲区的指针
 * len  接收到的IP报文长度
 * addr 发送ICMP报文响应的主机IP地址
 */ 
int unpack(char * buf, int len, char * addr)
{
   int i, ipheadlen;
   struct ip * ip;
   struct icmp * icmp;
   float rtt;          // 记录往返时间
   struct timeval end; // 记录接收报文的时间戳

   ip = (struct ip *)buf;

   // 计算ip首部长度，即ip首部的长度标识乘4
   ipheadlen = ip->hlen << 2;

   // 越过ip首部，指向ICMP报文
   icmp = (struct icmp *)(buf + ipheadlen);

   // ICMP报文的总长度
   len -= ipheadlen;

   // 如果小于ICMP报文首部长度8
   if(len < 8){
        fprintf(stderr,"ICMP packets\'s length is less than 8 \n"); 
        return -1;
   }

   // 确保是我们所发的ICMP ECHO回应
   if(icmp->type != ICMP_ECHOREPLY ||
           icmp->id != getpid()){    
       printf("ICMP packets are not send by us \n");
       return -1;
   }

   // 计算往返时间
   gettimeofday(&end, 0);
   rtt = timediff(&icmp->timestamp, &end);

   // 打印ttl，rtt，seq
   printf("%d bytes from %s : icmp_seq=%u ttl=%d rtt=%.2fms \n",
           len, addr, ((icmp->sequence)-1), ip->ttl, rtt);

   return 0;
}

int main(int argc, const char * argv[])
{
    char buf[BUF_SIZE] = {0};
    struct hostent *host;
    struct icmp sendicmp;
    struct sockaddr_in from;
    struct sockaddr_in to;
    int fromlen = 0;
    int sockfd;
    int nsend = 0;
    int nreceived = 0;
    int i, n;
    char str[16];

    bzero(&from,sizeof(struct sockaddr_in));
    bzero(&to,sizeof(struct sockaddr_in));

    if(argc < 2)
    {
        fprintf(stderr,"usage : %s hostname/IP address \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 生成原始套接字
    if((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
    {
        DEBUG("socket() error \n");
        exit(EXIT_FAILURE);
    }

    // 设置目的地址信息
    to.sin_family = AF_INET;

    // 判断是不是ip地址
    if(!inet_pton(AF_INET,argv[1],&to.sin_addr))
    {
        // 不是ip地址是域名
        if((host = gethostbyname(argv[1])) == NULL)
        {
            DEBUG("gethostbyname() error \n");
            exit(EXIT_FAILURE);
        }
        to.sin_addr = *(struct in_addr *)host->h_addr_list[0];
    }

    // 输出域名ip地址信息
    if(inet_ntop(AF_INET,&to.sin_addr.s_addr,str,sizeof(str)) != NULL)
    {
        printf("ping %s (%s) : %d bytes of data.\n", argv[1], str, (int)ICMP_SIZE);
    }
    
    //循环发送报文、接收报文 
    for(i = 0; i < NUM; i++)
    {
        nsend++;  // 发送次数加1
        bzero(&sendicmp,ICMP_SIZE);
        pack(&sendicmp, nsend);

        // 发送报文
        if(sendto(sockfd, &sendicmp, ICMP_SIZE, 0, (struct sockaddr *)&to, sizeof(to)) == -1)
        {
            DEBUG("sendto() error \n");
            continue;
        }

        // 接收报文
        if((n = recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&from, &fromlen)) < 0)
        {
            DEBUG("recvform() error \n");
            continue;
        }

        nreceived++;  // 接收次数加1
        if(nreceived > 1)//不解析第一次发送到0.0.0.0上的返回数据
        {
            if(inet_ntop(AF_INET,&from.sin_addr.s_addr,str,sizeof(str)) != NULL)
            {
                if(unpack(buf, n, str) == -1)
                {
                    DEBUG("unpack() error \n");
                }
            }
        }
        
        sleep(1);
    }

    // 输出统计信息
    printf("---  %s ping statistics ---\n", argv[1]);
    printf("%d packets transmitted, %d received, %%%d packet loss\n", (nsend-1), (nreceived-1), 
            (nsend - nreceived) / nsend * 100);

    return 0;
}

