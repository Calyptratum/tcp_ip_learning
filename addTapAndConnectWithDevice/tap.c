#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ether.h>

#define BUFFER_SIZE 1500
/*
 *标准以太网帧MTU值为1500，这个其实指的是数据链路层的有效载荷部分
 *以太网帧的结构如下：
 * 目的MAC地址：6字节
 *源MAC地址：6字节
 *类型字段：2字节
 *有效载荷（数据部分）：最大1500字节
 *帧检验序列（FCS）：4字节
 *因此，整个以太网帧的最大长度为1518字节（1500 + 18）
 *其中18字节是链路层的头部和尾部信息
 */

int open_tap(void){
    int tap_fd = open("/dev/net/tun",O_RDWR | O_NONBLOCK);
    if(tap_fd <0){
        perror("Opening /dev/net/tun");
        return -1;
    }
    return tap_fd;
}

int set_tap(struct ifreq* ifr,int tap_fd){
    
    memset(ifr,0,sizeof(*ifr));
    strncpy(ifr->ifr_name,"tap0",IFNAMSIZ);
    
    int ret = ioctl(tap_fd,TUNSETIFF,(void*)ifr);
    ifr->ifr_flags = IFF_TAP | IFF_NO_PI;
    if(ret <0){
        perror("TUNSETIFF");
        close(tap_fd);
        return -1;
    }
    printf("TAP device %s created \n",ifr->ifr_name);
    return 0;
}


int main(void){
    
    int tap_fd = open_tap();
    if(tap_fd < 0){
        exit(1);
    }
    
    struct ifreq ifr;
    int ret = set_tap(&ifr,tap_fd);
    if(ret <0){
        exit(-1);
    }

    char buffer[BUFFER_SIZE] = {0};
    struct ether_header *ether_header = (struct ether_header*)buffer;

    unsigned char dest_mac[6] ={0xff,0xff,0xff,0xff,0xff,0xff};
    memcpy(ether_header->ether_dhost,dest_mac,ETH_ALEN);

    
}
