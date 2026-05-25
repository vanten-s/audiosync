#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "packet.h"

// Caller needs to free the return value after sending
uint64_t serialize(struct Packet packet, uint8_t* buffer) {
    uint32_t data_length = 0;
    while (packet.data[data_length] != 0) 
        data_length++;

    int cursor = 0;
    for (int i = 0; i < 8; i++) {
        buffer[cursor] = packet.timestamp & 0xFF00000000000000;
        packet.timestamp <<= 8;
        printf("%x\n", packet.timestamp);
        cursor++;
    }

    buffer[cursor] = packet.packet_type;
    cursor++;

    for (int i = 0; i < 4; i++) {
        buffer[cursor] = packet.timestamp & 0xFF000000;
        data_length <<= 8;
        cursor++;
    }

    for (int i = 0; packet.data[i] != 0; i++) {
        buffer[cursor] = packet.data[i];
        cursor++;
    }

    buffer[cursor] = 0;
    cursor++;

    return cursor;
}
