#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <stdbool.h>
#include <netinet/in.h>

#include <vector.h>
#include <packet.h>

#define NUM_THREADS 32

void mainloop(
    struct sockaddr_in server_address
);

extern bool used_ids[NUM_THREADS];
extern pthread_mutex_t thread_socket[NUM_THREADS];
extern size_t read_pointers[NUM_THREADS];
extern size_t write_pointer;

extern uint8_t global_audio_buffer[1024][512];
extern struct Packet packet_buffer[1024];

void new_audio_data(uint8_t* audio_buffer, uint32_t data_length);
#endif
