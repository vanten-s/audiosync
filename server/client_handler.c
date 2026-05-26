#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <packet.h>

void handle_client(int fd, int client_id) {
    uint8_t buffer[MAX_PACKET_SIZE];
    int length = read(fd, buffer, MAX_PACKET_SIZE);
    if (length == -1) {
        printf("Read Failed");
        close(fd);
        return;
    }

    uint8_t data[MAX_PACKET_SIZE];
    struct Packet p = deserialize(buffer, data);

    
    printf("Client %d sent a packet:\n", client_id);
    debug_packet(p);
    
    close(fd);
}
