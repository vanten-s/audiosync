#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <netinet/in.h>
#include <vector.h>
#include <stdbool.h>

#define NUM_THREADS 32

void mainloop(
    struct sockaddr_in server_address
);

extern bool used_ids[NUM_THREADS];
extern pthread_mutex_t thread_socket[NUM_THREADS];

void new_audio_data(uint8_t* audio_buffer, uint32_t data_length);
#endif
