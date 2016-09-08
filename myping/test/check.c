/*************************************************************************
	> File Name: check.c
	> Author: AnSwEr
	> Mail: 
	> Created Time: 2015年07月17日 星期五 10时52分21秒
 ************************************************************************/

/*
 * addr 指向需校验数据缓冲区的指针
 * len 需校验数据的总长度（字节单位）
 */

 unsigned short checkSum(unsigned short *addr,int len)
 {
     unsigned int sum = 0;
     while(len > 1)
     {
         sum += *addr++;//16bit为单位
         len -= 2;//16bit=两个字节
     }

     //处理剩下的一个字节
     if(len == 1)
     {
         sum += *(unsigned char *)addr;
     }

     //将32位的高16位与低16位相加
     sum = (sum >> 16) + (sum & 0xffff);
     sum += (sum >> 16);

     return (unsigned short) ~sum;
 }
