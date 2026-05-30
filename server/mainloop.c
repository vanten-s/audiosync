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
size_t read_pointers[NUM_THREADS];

uint8_t global_audio_buffer[1024][512];
struct Packet packet_buffer[1024];
size_t write_pointer = 0;

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
    uint64_t audio_timestamp = current_time();
    memcpy(global_audio_buffer[write_pointer], audio_buffer, data_length);
    packet_buffer[write_pointer].timestamp = audio_timestamp;
    packet_buffer[write_pointer].packet_type = PACKET_TYPE_AUDIO_DATA;
    packet_buffer[write_pointer].data_length = data_length;
    packet_buffer[write_pointer].data = global_audio_buffer[write_pointer];

    write_pointer++;
    if (write_pointer == 1024) {
        write_pointer = 0;
    }
}
