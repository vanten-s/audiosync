#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <packet.h>
#include <client_handler.h>

void free_client(struct Client* client) {
    close(client->fd);

}

void* handle_client(void* c) {
    struct Client* client = (struct Client*)c;
    int fd = client->fd;
    int client_id = client->client_id;

    uint8_t buffer[MAX_PACKET_SIZE];
    int length = read(fd, buffer, MAX_PACKET_SIZE);
    if (length == -1) {
        printf("Read Failed");
        free_client(client);
        return NULL;
    }

    uint8_t data[MAX_PACKET_SIZE];
    struct Packet p = deserialize(buffer, data);
    
    printf("Client %d sent a packet:\n", client_id);
    debug_packet(p);
    
    free_client(client);
    return NULL;
}
