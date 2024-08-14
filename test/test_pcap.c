#include <pcap.h>
#include <stdio.h>

int main() {
    char *dev, errbuf[PCAP_ERRBUF_SIZE];

    // 查找默认设备
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
        return 2;
    }
    printf("Device: %s\n", dev);
    return 0;
}
