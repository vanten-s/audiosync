#include <portaudio.h>
#include <mainloop.h>
#include <stdio.h>

/*
typedef int PaStreamCallback( const void *input,
                                      void *output,
                                      unsigned long frameCount,
                                      const PaStreamCallbackTimeInfo* timeInfo,
                                      PaStreamCallbackFlags statusFlags,
                                      void *userData ) ;
*/

int stream_callback(const void *input, void *output,
                    unsigned long frameCount,
                    const PaStreamCallbackTimeInfo* timeInfo,
                    PaStreamCallbackFlags statusFlags,
                    void *userData) 
{
    new_audio_data((uint8_t*)input, 2*frameCount);
    return 0;
}

void source_portaudio_init() {
    if (Pa_Initialize() != paNoError) {
        printf("PortAudio error!\n");
        return;
    }
    printf("PortAudio initialized!\n");

    PaStream *stream;
    PaError err;
    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream( &stream,
                                1,          /* mono input */
                                0,          /* no output channels */
                                paInt16,    /* 16 bit audio */
                                48000,
                                256,        /* frames per buffer, i.e. the number
                                                   of sample frames that PortAudio will
                                                   request from the callback. Many apps
                                                   may want to use
                                                   paFramesPerBufferUnspecified, which
                                                   tells PortAudio to pick the best,
                                                   possibly changing, buffer size.*/
                                stream_callback, /* this is your callback function */
                                NULL ); /*This is a pointer that will be passed to
                                                   your callback*/
    if( err != paNoError ) {
        printf("Error initializing stream!\n");
        return;
    }
    printf("Audio stream opened!\n");

    if (Pa_StartStream(stream) != paNoError) {
        printf("failed to start stream\n");
        return;
    }
    printf("Audio stream started!\n");
}

