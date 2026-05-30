#ifndef AUDIO_H
#define AUDIO_H
#include <stdbool.h>
#include <packet.h>

void handle_audio_data(struct Packet p);
void audio_init();

struct AudioFrame {
    uint64_t timestamp;
    uint16_t audio_data[256];
};
#endif
