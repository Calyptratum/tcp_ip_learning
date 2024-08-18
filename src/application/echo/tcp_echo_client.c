#include <string.h>
#include "netapi.h"
#include "tcp_echo_client.h"
#include "sys_plat.h"

 /**
  * @brief TCP回显客户端程序
  */
int tcp_echo_client_start (const char* ip, int port) {
    plat_printf("tcp echo client, ip: %s, port: %d\n", ip, port);
    plat_printf("Enter quit to exit\n");

    // 创建套接字，使用流式传输，即tcp
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        plat_printf("tcp echo client: open socket error");
        goto end;
    }

    // 设置超时
    struct timeval tmo;
    tmo.tv_sec = 120;
    tmo.tv_usec = 0;
    setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tmo, sizeof(tmo));
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tmo, sizeof(tmo));

    // 创建套接字，使用流式传输，即tcp
    struct sockaddr_in server_addr;
    plat_memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);   // ip地址，注意大小端转换
    server_addr.sin_port = htons(port);             // 取端口号，注意大小端转换
    if (connect(s, (const struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        plat_printf("connect error");
        goto end;
    }

    // 循环，读取一行后发出去
    char buf[128];
    plat_printf(">>");
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        // 将数据写到服务器中，不含结束符
        if (send(s, buf, plat_strlen(buf) - 1, 0) <= 0) {
            plat_printf("write error");
            goto end;
        }

        // 读取回显结果并显示到屏幕上，不含结束符
        plat_memset(buf, 0, sizeof(buf));
        int len = recv(s, buf, sizeof(buf) - 1, 0);
        if (len <= 0) {
            plat_printf("read error");
            goto end;
        }
        buf[sizeof(buf) - 1] = '\0';        // 谨慎起见，写结束符

        // 在屏幕上显示出来
        plat_printf("%s", buf);
        plat_printf(">>");
    }
    close(s);
    return 0;

end:
    if (s >= 0) {
        close(s);
    }
    return -1;
}


