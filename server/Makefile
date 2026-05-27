CC = gcc

OBJS = main.o \
	   mainloop.o \
	   client_handler.o \
	   sources/random.o \
	   sources/portaudio.o

INCLUDE = -I../common \
		  -Iinclude

LDFLAGS := -L../common -lportaudio
CFLAGS := -Wall \
		  $(INCLUDE) 

.PHONY: common all run clean

all: server

common: 
	cd ../common && $(MAKE) libaudiosync.a

server: $(OBJS) common
	gcc -o server $(OBJS) $(LDFLAGS) -laudiosync

run: all
	./server

clean:
	rm -f $(OBJS)
	rm -f server
	cd ../common && $(MAKE) clean
