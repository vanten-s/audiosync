CC = gcc

OBJS = main.o \
	   audio.o

INCLUDE = -I../common \
		  -Iinclude

LDFLAGS := -L../common \
		   -lportaudio

CFLAGS := -Wall \
		  $(INCLUDE) 

LIBFLAGS :=

.PHONY: common all run clean debug

debug: LIBFLAGS += debug
debug: CFLAGS += -g
debug: all

all: server

common: 
	cd ../common && $(MAKE) $(LIBFLAGS)

server: $(OBJS) common
	gcc -o client $(OBJS) $(LDFLAGS) -laudiosync

run: all
	./client

clean:
	rm -f $(OBJS)
	rm -f client
	cd ../common && $(MAKE) clean
