#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "dbg.h"
//#include "net_plat.h"

/**
 * @brief 调试信息输出
 * 输出文件名、函数名、函数以及调试信息
 */
void dbg_print(int m_level, int s_level, const char* file, const char* func, int line, const char* fmt, ...) {
    static const char* title[] = {
        [DBG_LEVEL_ERROR] = DBG_STYLE_ERROR"error",
        [DBG_LEVEL_WARNING] = DBG_STYLE_WARNING"warning",
        [DBG_LEVEL_INFO] = "info",
        [DBG_LEVEL_NONE] = "none"
    };

    // 当仅当前等级数值比较大时才输出
    if (m_level >= s_level) {
        // 定位到文件名部分，因为file传输的是完整的文件路径，太长了
        const char * end = file + strlen(file);
        while (end >= file) {
            if ((*end == '\\') || (*end == '/')) {
                break;
            }
            end--;
        }
        end++;

        // 每行信息提示的开头
        printf("%s(%s-%s-%d):", title[s_level], end, func, line);

        char str_buf[128];
        va_list args;

        // 具体的信息
        va_start(args, fmt);
        vsprintf(str_buf, fmt, args);
        printf("%s\n"DBG_STYLE_RESET, str_buf);
        va_end(args);
    }
}

/**
 * 打印mac地址
 */
void dump_mac(const char * msg, const uint8_t * mac) {
    if (msg) {
        printf("%s", msg);
    }

    plat_printf("%02x-%02x-%02x-%02x-%02x-%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/**
 * 打印IP地址
 * @param ip ip地址
 */
void dump_ip_buf(const char* msg, const uint8_t* ip) {
    if (msg) {
        printf("%s", msg);
    }

    if (ip) {
        plat_printf("%d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
    } else {
        printf("0.0.0.0\n");
    }
}
