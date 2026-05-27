#ifndef AUDIO_H
#define AUDIO_H
#include <packet.h>

void handle_audio_data(struct Packet p);

struct AudioFrame {
    uint16_t audio_data[256];
};
#endif
