CC = gcc

OBJS = main.o \
	   mainloop.o \
	   client_handler.o \
	   sources/random.o \
	   sources/portaudio.o

INCLUDE = -I../common \
		  -Iinclude

LDFLAGS := -L../common \
		   -lportaudio
CFLAGS := -Wall \
		  $(INCLUDE) 

LIBFLAGS :=

.PHONY: common all run clean debug

all: server

debug: CFLAGS += -g
debug: LIBFLAGS += debug
debug: all

common: 
	cd ../common && $(MAKE) $(LIBFLAGS)

server: $(OBJS) common
	gcc -o server $(OBJS) $(LDFLAGS) -laudiosync

run: all
	./server

clean:
	rm -f $(OBJS)
	rm -f server
	cd ../common && $(MAKE) clean
