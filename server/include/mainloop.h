#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <netinet/in.h>
#include <vector.h>
#include <stdbool.h>

#define NUM_THREADS 32

void mainloop(
    struct sockaddr_in server_address,
    struct Source source
);

extern bool used_ids[NUM_THREADS];
#endif
