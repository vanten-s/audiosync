#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "packet.h"

// Caller needs to free the return value after sending
int serialize(struct Packet packet, uint8_t* buffer) {
    int cursor = 0;

    int cursor_start = cursor; // Keep track of position in loop since it runs backwards
    for (int i = cursor + 7; i >= cursor_start; i--) {
        buffer[i] = packet.timestamp & 0xFF;
        packet.timestamp >>= 8;
        cursor++;
    }

    buffer[cursor] = packet.packet_type;
    cursor++;

    uint32_t data_length = packet.data_length; // Used because packet.data_length gets consumed by the for loop
    cursor_start = cursor; // Keep track of position in loop since it runs backwards
    for (int i = cursor + 3; i >= cursor_start; i--) {
        buffer[i] = packet.data_length & 0xFF;
        packet.data_length >>= 8;
        cursor++;
    }

    for (int i = 0; i < data_length; i++) {
        buffer[cursor] = packet.data[i];
        cursor++;
    }

    return cursor;
}

// Caller needs to free the return value after sending
struct Packet deserialize(uint8_t* serialized, uint8_t* data_buffer) {
    int cursor = 0;

    uint64_t timestamp;
    for (int i = 0; i < 8; i++) {
        timestamp <<= 8;
        timestamp |= serialized[cursor];
        cursor++;
    }

    enum PacketType packet_type = serialized[cursor];
    cursor++;

    uint32_t data_length;
    for (int i = 0; i < 4; i++) {
        data_length <<= 8;
        data_length |= serialized[cursor];
        cursor++;
    }

    printf("%x\n", data_length);

    for (int i = 0; i < data_length; i++) {
        data_buffer[i] = serialized[cursor];
        cursor++; 
    }

    struct Packet p = { 
        .timestamp = timestamp,
        .packet_type = packet_type,
        .data_length = data_length,
        .data = data_buffer,
    };

    return p;
}
