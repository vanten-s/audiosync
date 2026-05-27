#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <packet.h>
#include <timer.h>
#include <client_handler.h>
#include <mainloop.h>

int initial_connection(struct Client* client) {
    int fd = client->fd;
    int client_id = client->client_id;

    uint8_t buffer[MAX_PACKET_SIZE];
    uint8_t data[MAX_PACKET_SIZE];
    int length = read(fd, buffer, MAX_PACKET_SIZE);
    
    struct Packet p = deserialize(buffer, data);
    if (p.packet_type != PACKET_TYPE_CONNECTION_REQUEST) {
        printf("Invalid packet received from %d\n", client_id);
        return -1;
    }

    p.timestamp = current_time(); 
    p.packet_type = PACKET_TYPE_SYNCING_REQUEST; 
    p.data_length = 0; 
    p.data = NULL; 

    // Send Syncing request
    length = serialize(p, buffer);
    uint64_t time_sent = current_time();
    write(fd, buffer, length);
    read(fd, buffer, MAX_PACKET_SIZE);
    uint64_t time_received = current_time();

    uint64_t time_delay = (time_received - time_sent) >> 1;

    p = deserialize(buffer, data);
    if (p.packet_type != PACKET_TYPE_SYNCING_RESPONSE) {
        printf("Invalid packet received from %d\n", client_id);
        return -1;
    }
    
    p.packet_type = PACKET_TYPE_CORRECTED_TIMESTAMP;
    p.data_length = 0;
    p.data = NULL;
    p.timestamp = current_time() - time_delay;

    length = serialize(p, buffer);
    write(fd, buffer, length);

    printf("Client number %d has %ld ticks delay and connected on tick %ld\n", client_id, time_delay, time_sent);
    
    pthread_mutex_unlock(&thread_socket[client_id]);

    return 0;
}

void free_client(struct Client* client) {
    close(client->fd);
    free(client);
}

void* handle_client(void* c) {
    struct Client* client = (struct Client*)c;
    int fd = client->fd;
    int client_id = client->client_id;

    if (initial_connection(client) == -1) {
        free_client(client);
        return NULL;
    }

    while (1) {
        sleep(1);
    }
    
    free_client(client);
    return NULL;
}
