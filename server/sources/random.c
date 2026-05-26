#include <stdint.h>
#include <vector.h>
#include <stdlib.h>

uint32_t static callback(uint16_t buffer_length, uint16_t* buffer) {
    for (int i = 0; i < buffer_length; i++) {
        buffer[i] = rand();
    }
    return buffer_length;
}

struct Source random_source() {
    struct Source source;
    source.callback = callback;
    return source;
}

