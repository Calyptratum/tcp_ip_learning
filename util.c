#include "util.h"
#include "memory.h"
struct queue_entry {
    struct queue_entry *next;
    void *data;
};

void
queue_init(struct queue_head *queue)
{
    queue->head = NULL;
    queue->tail = NULL;
    queue->num = 0;
}

void *
queue_push(struct queue_head *queue, void *data)
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

void *
queue_pop(struct queue_head *queue)
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

void *
queue_peek(struct queue_head *queue)
{
    if (!queue || !queue->head) {
        return NULL;
    }
    return queue->head->data;
}
