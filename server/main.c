#include <stdio.h>
#include <stdlib.h>
#include "packet.h"

int main(int argc, char const* argv[]) {
    struct Packet pack;
    pack.timestamp = 0xff;
    pack.packet_type = PACKET_TYPE_CONNECTION_REQUEST;
    pack.data = (uint8_t*)"Hello, World!\n";
    uint8_t* buffer = malloc(256);
    uint64_t length = serialize(pack, buffer);
    
    for (int i = 0; i < length; i++) {
        printf("%x", buffer[i]);
    }
    printf("\n");

    free(buffer);
}
