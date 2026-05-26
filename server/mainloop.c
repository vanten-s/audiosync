#include <stdint.h>
#include <stdio.h>
#include <vector.h>
#include <mainloop.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void mainloop(
    struct sockaddr_in server_address,
    struct Source source
) {
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
        int connetion_file_descriptor = accept(socket_file_descriptor, (struct sockaddr*)&client_address, (socklen_t*)&client_address_len);
    }
    printf("Program Exited");
}
