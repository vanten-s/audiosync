#include <stdint.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include <vector.h>
#include <packet.h>
#include <timer.h>

#include <mainloop.h>
#include <client_handler.h>

// Primarily set by mainloop
pthread_t threads[NUM_THREADS];
int file_descriptors[NUM_THREADS];
bool used_ids[NUM_THREADS];

// Primarily set by threads
uint64_t delays[NUM_THREADS];
pthread_mutex_t thread_socket[NUM_THREADS];

uint8_t audio_buffer_1[256];
uint8_t audio_buffer_2[256];
bool using_audio_buffer_1 = true;
bool last_checked_buffer_status = true;
uint64_t audio_timestamp;

void* audio_sender(void* arg) {
    while (1) {
        if (using_audio_buffer_1 == last_checked_buffer_status) { usleep(2000); continue; }
        last_checked_buffer_status= using_audio_buffer_1;

        struct Packet p;
        p.timestamp = audio_timestamp;
        p.packet_type = PACKET_TYPE_AUDIO_DATA;
        p.data_length = 256;
        if (using_audio_buffer_1) {
            p.data = audio_buffer_1;
        } else {
            p.data = audio_buffer_2;
        }

        uint8_t packet_buffer[1024];
        int packet_length = serialize(p, packet_buffer);

        for (int i = 0; i < NUM_THREADS; i++) {
            if (used_ids[i] != true) { continue; }

            if (pthread_mutex_lock(&thread_socket[i]) != 0) {
                printf("Couldn't get lock for client %d\n", i);
                continue;
            }

            int length_written = write(file_descriptors[i], packet_buffer, packet_length);
            if (length_written != packet_length) {
                printf("Writing to client %d failed\n", i);
            } else {
                printf("Sent audio data to client %d!\n", i);
            }

            if (pthread_mutex_unlock(&thread_socket[i]) != 0) {
                printf("Couldn't unlock client %d\n", i);
            }
        }
    }
}

void mainloop(struct sockaddr_in server_address) {
    for (int i = 0; i < NUM_THREADS; i++) {
        used_ids[i] = false;
    }

    int socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1) {
        printf("Failed creating socket...\n");
        return;
    }
    printf("Socket created\n");

    // Binds the socket to the passed address
    if (bind(socket_file_descriptor, (struct sockaddr*)&server_address, sizeof(server_address)) != 0) {
        printf("Socket binding failed. (Address in use maybe?)\n");
        return;
    }
    printf("Socket binded\n");

    // Maximum of 5 queued connections.
    if (listen(socket_file_descriptor, 5) != 0) {
        printf("Listening failed\n");
        return;
    }
    
    printf("Server now listening.\n");

    pthread_t audio_sender_thread;
    pthread_create(&audio_sender_thread, NULL, audio_sender, NULL);

    while (1) {
        struct sockaddr_in client_address;
        int client_address_len = sizeof(client_address);
        int connection_file_descriptor = accept(socket_file_descriptor, (struct sockaddr*)&client_address, (socklen_t*)&client_address_len);

        int client_id = 0;
        while (used_ids[client_id] != false && client_id != NUM_THREADS) {
            client_id++;
        }
        if (client_id == NUM_THREADS) {
            printf("All client slots occupied\n");
            close(connection_file_descriptor);
            continue;
        }

        used_ids[client_id] = true;
        pthread_mutex_init(&thread_socket[client_id], NULL);
        pthread_mutex_lock(&thread_socket[client_id]);
        file_descriptors[client_id] = connection_file_descriptor;
        printf("Client number %d connected\n", client_id);

        struct Client* c = malloc(sizeof(c));
        c->fd = connection_file_descriptor; 
        c->client_id = client_id;
        pthread_create(threads+client_id, NULL, handle_client, c);
    }
    printf("Program Exited\n");
}

// This will be called by the source every time a new frame presents itself.
void new_audio_data(uint8_t* audio_buffer, uint32_t data_length) {
    if (using_audio_buffer_1) {
        memcpy(audio_buffer_2, audio_buffer, data_length);
    } else {
        memcpy(audio_buffer_1, audio_buffer, data_length);
    }
    audio_timestamp = current_time();
    using_audio_buffer_1 = !using_audio_buffer_1;
}
