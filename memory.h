#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdlib.h>

inline void* memory_alloc(size_t size){
    return calloc(1,size);
} 

inline void memory_free(void* ptr){
    free(ptr);
}

#endif
