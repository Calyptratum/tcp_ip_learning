#ifndef  __UTIL_H__
#define  __UTIL_H__

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

struct queue_entry;

struct queue_head{
    struct queue_entry* head;
    struct queue_entry* tail;
    unsigned int num;
};

void queue_init(struct queue_head* queue);

void* queue_push(struct queue_head* queue,void* data);

void* queue_pop(struct queue_head* queue);

void* queue_peek(struct queue_head* queue);

void queue_foreach(struct queue_head* queue,void (*func)(void *arg, void *data), void *arg);


uint16_t swap16(uint16_t num);

uint32_t swap32(uint32_t num);

#define hton16(num) swap16(num);
#define ntoh16(num) swap16(num);
#define hton32(num) swap32(num);
#define ntoh32(num) swap32(num);

net_err_t tools_init(void);
uint16_t checksum16(uint32_t offset, void* buf, uint16_t len, uint32_t pre_sum, int complement);

#endif
