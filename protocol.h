#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#define NET_PROT_DYN_START 1024 //动态端口，0~1023端口通常被系统进程或者特定程序占用

#define NET_PROT_DYN_END 65535  //最大端口

typedef enum _port_t{
    /*
     * 0号端口有特殊意义
     * 在unix socket编程中被用作动态端口分配的指示符
     * 允许操作系统自动选择一个可用端口
     */
    NET_PROT_EMPTY = 0, 

}port_t;

/*
 * 常见协议
 */

typedef enum _protocol_t{
    NET_PROTOCOL_ARP = 0x0806, //ARP协议
    NET_PROTOCOL_IPv4 = 0x0800, //ipv4
    NET_PROTOCOL_ICMPv4 = 0x1, //ICMP
    NET_PROTOCOL_UDP = 0x11,  //UDP
    NET_PROTOCOL_TCP = 0x06,  //TCP

}protocol_t;

#endif  //__PROTOCOL_H__
