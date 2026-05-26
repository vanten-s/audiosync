#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "packet.h"

int main(int argc, char const* argv[]) {
    struct Packet pack;
    pack.timestamp = 0x124358671232abff;
    pack.packet_type = PACKET_TYPE_AUDIO_DATA;
    pack.data = (uint8_t*)"Hello, World!\n";
    pack.data_length = strlen((char*)pack.data);
    printf("Data length: %x\n", pack.data_length);
    uint8_t* buffer = malloc(256);
    uint64_t length = serialize(pack, buffer);
    
    for (int i = 0; i < length; i++) {
        printf("%02x", buffer[i]);
    }
    printf("\n");

    uint8_t* data_buf = malloc(256);
    struct Packet p = deserialize(buffer, data_buf); 

    char* debug_buf = malloc(256);
    packet_to_debug_string(p, debug_buf);
    printf("%s", debug_buf);
    free(debug_buf);

    debug_buf = malloc(256);
    packet_to_debug_string(pack, debug_buf);
    printf("%s", debug_buf);
    free(debug_buf);

    free(data_buf);
    free(buffer);
}
