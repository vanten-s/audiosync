#ifndef VECTOR_H
#define VECTOR_H

#include <pthread.h>
#include <stdint.h>

struct ThreadState {
    pthread_t* threads;    
};

struct Source {
    uint16_t buffer[1024]; // PCM encoded audio
    uint32_t (*callback)(uint16_t, uint16_t*); // Takes in a buffer and its length, fills the buffer with audio-data from the source and then returns the number of samples filled
};
#endif

