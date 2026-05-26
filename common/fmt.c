#include <stdbool.h>
#include <stdio.h>
#include "packet.h"

char* packet_types[] = {
    "PACKET_TYPE_CONNECTION_REQUEST",
    "PACKET_TYPE_CONNECTION_RESPONSE",
    "PACKET_TYPE_SYNCING_REPONSE",
    "PACKET_TYPE_CORRECTED_TIMESTAMP",
    "PACKET_TYPE_AUDIO_DATA",
    "PACKET_TYPE_AUDIO_DATA_ACK",
};

uint32_t packet_to_debug_string(struct Packet packet, char* buffer) {
    return sprintf(buffer, "Timestamp: %016lx\nType: %s\nData Length: %08x\n", packet.timestamp, packet_types[packet.packet_type], packet.data_length);
}
