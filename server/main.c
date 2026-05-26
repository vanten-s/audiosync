#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "packet.h"
#include "mainloop.h"

#include "sources/random.h"

int main(int argc, char const* argv[]) {
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr)); // Zero out the address

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Allow any in-address
    servaddr.sin_port = htons(5725); // Port 5725, htons convert it to correct bit order

    struct Source source = random_source();

    mainloop(
        servaddr,
        source
    );
}
