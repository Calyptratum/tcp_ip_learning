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

#endif
