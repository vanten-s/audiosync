#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <netinet/in.h>
#include <stdint.h>
#include <vector.h>

void mainloop(
    struct sockaddr_in server_address,
    struct Source source
);
#endif
