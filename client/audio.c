#include <portaudio.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <packet.h>
#include <timer.h>
#include <main.h>

#include <audio.h>
#define BUFFER_LENGTH 4196
#define LAST_PLAYED_PACKETS_LENGTH 16

#define LATENCY_MS 100
#define LATENCY_US (1000 * LATENCY_MS)

struct AudioFrame buffer_list[BUFFER_LENGTH];
size_t write_pointer = 0;
size_t read_pointer = 0;

// Keeps tracked of the last played packets, and makes sure that the program doesn't repeat the same packets twice.
size_t last_played_packets[LAST_PLAYED_PACKETS_LENGTH]; 

int stream_callback(const void* input,
                    void* output,
                    unsigned long framecount,
                    const PaStreamCallbackTimeInfo* timeInfo,
                    PaStreamCallbackFlags statusFlags,
                    void *userData ) 
{

    size_t i = write_pointer - 1;
    if (i == -1) {
        bzero(output, 512);
        return 0;
    }
    uint64_t timestamp = current_time() + clock_sync_difference - LATENCY_US;

    while (timestamp <= buffer_list[i].timestamp && i != write_pointer /* prevents infinite loops */) {
        i--;
        if (i == 0) {
            i = BUFFER_LENGTH - 1;
        }
    }

    for (int j = 0; j < LAST_PLAYED_PACKETS_LENGTH - 1; j++) {
        last_played_packets[j] = last_played_packets[j + 1];
        if (i == last_played_packets[j]) { i++; }
    }

    last_played_packets[LAST_PLAYED_PACKETS_LENGTH - 1] = i;

    if (i >= BUFFER_LENGTH) {
        i -= BUFFER_LENGTH;
    }

    memcpy(output, buffer_list[i].audio_data, 512);

    return 0;
}

void audio_init() {
    if (Pa_Initialize() != paNoError) {
        printf("Failed to initialize portaudio\n");
        return;
    }

    PaStream* stream;
    PaError err;
    err = Pa_OpenDefaultStream(
            &stream, 
            0, 
            1, 
            paInt16,
            48000, 
            256, 
            stream_callback, 
            NULL
    );

    if (err != paNoError) {
        printf("Failed opening stream\n");
        return;
    }

    printf("Audio stream opened!\n");

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        printf("Error while starting stream\n");
        printf("Error text: %s\n", Pa_GetErrorText(err));
        printf("Error id: %d\n", err);
        return;
    }

    printf("Started audio stream!\n");
}

void handle_audio_data(struct Packet p) {
    write_pointer++;
    if (write_pointer == BUFFER_LENGTH) {
        write_pointer = 0;
    }
    memcpy(buffer_list[write_pointer].audio_data, p.data, 512);
    buffer_list[write_pointer].timestamp = p.timestamp;
}

