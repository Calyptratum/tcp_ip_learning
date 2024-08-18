#ifndef __PLAT_H__
#define __PLAT_H__

#include <errno.h>
#include <pcap.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

//为了使得可以用其他编译器编译
#ifndef __GNUC__
typedef long size_t;
#endif

typedef struct _xsys_sem_t{
    int count;                //信号计数
    pthread_cond_t cond;      //条件变量
    pthread_mutex_t locker;   //访问互斥锁
}sys_sem_t;

typedef  struct timeval net_time_t;  //时间类型

#define SYS_THREAD_INVALID    (pthread_t)0
/*定义一个无效的线程标识符，
 *当创建失败，将线程ID设置未这个状态标识失败或无效
 */

#define SYS_SEM_INVALID       (sys_sem_t*)0  //定义一个无效的信号量标识符

#define SYS_MUTEX_INVALID     (pthread_mutex_t*)0 //定义一个无效的互斥锁标识符

//网卡相关函数
int pcap_find_device(const char* ip, char* name_buf);
int pcap_show_list(void);
pcap_t* pacp_device_open(const char* ip ,const uint8_t* mac_addr);

sys_sem_t* sys_sem_create(int init_count);
void sys_sem_free(sys_sem_t* sem);
void sys_sem_wait(sys_sem_t* sem, uint32_t ms);
void sys_sem_notify(sys_sem_t* sem);


pthread_mutex_t* sys_mutex_cerate(void);
void sys_mutex_free(pthread_mutex_t* mutex);
void sys_mutex_lock(pthread_mutex_t* mutex);
void sys_mutex_unlock(pthread_mutex_t* mutex);
int sys_mutex_isvalid(pthread_mutex_t* mutex);

typedef  void (*sys_thread_func)(void* arg);
pthread_t sys_thread_create(sys_thread_func entry,void* arg);
void sys_thread_exit(int error);
void sys_sleep(int ms);
pthread_t sys_thread_self(void);


inline void* memory_alloc(size_t size){
    return calloc(1,size);
} 

inline void memory_free(void* ptr){
    free(ptr);
}



#endif
