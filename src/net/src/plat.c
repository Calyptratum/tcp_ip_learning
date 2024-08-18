#include "plat.h"

#include <arpa/inet.h>
#include <netinet/in.h>

void sys_time_curr(net_time_t* time){
    gettimeofday(time ,NULL);
}

