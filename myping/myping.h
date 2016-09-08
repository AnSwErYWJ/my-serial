/*************************************************************************
	> File Name: myping.h
	> Author: AnSwEr
	> Mail: 1045837697@qq.com
	> Created Time: 2015年07月17日 星期五 11时22分21秒
 ************************************************************************/

#ifndef _MYPING_H
#define _MYPING_H

#define ICMP_SIZE (sizeof(struct icmp))
#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0
#define BUF_SIZE 1024
#define NUM   6    // 发送报文次数
#define DEBUG_PRINT 1

// ICMP报文数据结构
struct icmp{
    unsigned char type;      // 类型
    unsigned char code;      // 代码
    unsigned short checksum;  // 校验和
    unsigned short id;        // 标识符
    unsigned short sequence;  // 序号 
    struct timeval timestamp; // 时间戳
};


// IP首部数据结构
struct ip{
    // 主机字节序判断
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned char   hlen:4;        // 首部长度
    unsigned char   version:4;     // 版本      
    #endif
    #if __BYTE_ORDER == __BIG_ENDIAN
    unsigned char   version:4;       
    unsigned char   hlen:4;    
    #endif    
    unsigned char   tos;             // 服务类型
    unsigned short  len;             // 总长度
    unsigned short  id;                // 标识符
    unsigned short  offset;            // 标志和片偏移
    unsigned char   ttl;            // 生存时间
    unsigned char   protocol;       // 协议
    unsigned short  checksum;       // 校验和
    struct in_addr ipsrc;    // 32位源ip地址
    struct in_addr ipdst;   // 32位目的ip地址
};

unsigned short checkSum(unsigned short *, int); // 计算校验和
float timediff(struct timeval *, struct timeval *); // 计算时间差
void pack(struct icmp *, int);  // 封装一个ICMP报文
int unpack(char *, int, char *);        // 对接收到的IP报文进行解包


unsigned short checkSum(unsigned short *addr, int len);
float timediff(struct timeval *begin, struct timeval *end);
void pack(struct icmp * icmp, int sequence);
int unpack(char * buf, int len, char * addr);

#endif
