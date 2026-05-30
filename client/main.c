#include <stdio.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <strings.h> 
#include <unistd.h>

#include <packet.h> 
#include <timer.h> 

#include <audio.h> 

int64_t clock_sync_difference;

void initial_connection(int fd) {
    struct Packet p;
    p.timestamp = 0;
    p.packet_type = PACKET_TYPE_CONNECTION_REQUEST;
    p.data_length = 0;
    p.data = NULL;

    uint8_t buffer[2048];
    uint8_t data[2048];
    int length = serialize(p, buffer);

    write(fd, buffer, length);

    // Receive connection response
    length = read(fd, buffer, 2048);

    p.timestamp = 0;
    p.packet_type = PACKET_TYPE_SYNCING_RESPONSE;
    p.data_length = 0;
    p.data = NULL;

    length = serialize(p, buffer);
    write(fd, buffer, length);

    length = read(fd, buffer, 2048);
    uint64_t time_sync_received = current_time();
    p = deserialize(buffer, data, 2048, 2048);
    uint64_t sync_timestamp_received = p.timestamp;

    clock_sync_difference = sync_timestamp_received - time_sync_received;
}

int main() {
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(5725);

    audio_init();
        
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        printf("Socket creation failed\n");
        return 1;
    }
    printf("Socket created.\n");

    if (connect(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) != 0) {
        printf("Connection with the server failed\n");
        return 1;
    }

    printf("Connected to the server!\n");

    initial_connection(socket_fd);
    printf("Server and client synced with time difference: %ld\n", clock_sync_difference);

    uint8_t buffer[2048];
    uint8_t data_buffer[2048];
    while (1) {
        int length = read(socket_fd, buffer, 525);
        if (length == 0) {
            break;
        } else if (length == -1) {
            printf("Error in read");
            break;
        }

        struct Packet p = deserialize(buffer, data_buffer, 525, 2048);

        switch (p.packet_type) {
            case PACKET_TYPE_AUDIO_DATA:
                handle_audio_data(p);
                break;

            default:
                printf("Invalid Packet, type: %d\n", p.packet_type);
        }
    }

    close(socket_fd);
}
