#include "util.h"
#include "plat.h"
struct queue_entry {
    struct queue_entry *next;
    void *data;
};

void queue_init(struct queue_head *queue)
{
    queue->head = NULL;
    queue->tail = NULL;
    queue->num = 0;
}

void *queue_push(struct queue_head *queue, void *data)
{
    struct queue_entry *entry;

    if (!queue) {
        return NULL;
    }
    entry = (struct queue_entry*)memory_alloc(sizeof(*entry));
    if (!entry) {
        return NULL;
    }
    entry->next = NULL;
    entry->data = data;
    if (queue->tail) {
        queue->tail->next = entry;
    }
    queue->tail = entry;
    if (!queue->head) {
        queue->head = entry;
    }
    queue->num++;
    return data;
}

void *queue_pop(struct queue_head *queue)
{
    struct queue_entry *entry;
    void *data;

    if (!queue || !queue->head) {
        return NULL;
    }
    entry = queue->head;
    queue->head = entry->next;
    if (!queue->head) {
        queue->tail = NULL;
    }
    queue->num--;
    data = entry->data;
    memory_free(entry);
    return data;
}

void *queue_peek(struct queue_head *queue)
{
    if (!queue || !queue->head) {
        return NULL;
    }
    return queue->head->data;
}


uint16_t swap16(uint16_t num){
    uint16_t r;
    r = (num & 0x00ff)<<8 | (num & 0xff00)>>8;
    return r;
}

uint32_t swap32(uint32_t num){
    uint32_t r;
    r =   (num & 0x000000ff)<<24
        | (num & 0x0000ff00)<<8
        | (num & 0x00ff0000)>>8
        | (num & 0xff000000)>>24;
         
    return r;
}

uint16_t checksum16(uint32_t offset, void* buf, uint16_t len, uint32_t pre_sum, int complement) {
    uint16_t* curr_buf = (uint16_t *)buf;
    uint32_t checksum = pre_sum;

    // 起始字节不对齐, 加到高8位
    if (offset & 0x1) {
        uint8_t * buf = (uint8_t *)curr_buf;
        checksum += *buf++ << 8;
        curr_buf = (uint16_t *)buf;
        len--;
    }

    while (len > 1) {
        checksum += *curr_buf++;
        len -= 2;
    }

    if (len > 0) {
        checksum += *(uint8_t*)curr_buf;
    }

    // 注意，这里要不断累加。不然结果在某些情况下计算不正确
    uint16_t high;
    while ((high = checksum >> 16) != 0) {
        checksum = high + (checksum & 0xffff);
    }

    return complement ? (uint16_t)~checksum : (uint16_t)checksum;
}

