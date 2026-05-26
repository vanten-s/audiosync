#include <stdint.h>

enum PacketType {
    PACKET_TYPE_CONNECTION_REQUEST  = 0,
    PACKET_TYPE_CONNECTION_RESPONSE = 1,
    PACKET_TYPE_SYNCING_REPONSE     = 2,
    PACKET_TYPE_CORRECTED_TIMESTAMP = 3,
    PACKET_TYPE_AUDIO_DATA          = 4,
    PACKET_TYPE_AUDIO_DATA_ACK      = 5,
};

struct Packet {
    uint64_t timestamp; // time in us since server started
    enum PacketType packet_type; // time in us since server started
    uint32_t data_length;
    uint8_t* data;
};

int serialize(struct Packet packet, uint8_t* buffer);
struct Packet deserialize(uint8_t* buffer, uint8_t* data_buffer);
uint32_t packet_to_debug_string(struct Packet packet, char* buffer);

