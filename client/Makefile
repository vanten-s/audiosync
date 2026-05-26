CC = gcc

OBJS = main.o

INCLUDE = -I../common \
		  -Iinclude

LDFLAGS := -L../common
CFLAGS := -Wall \
		  $(INCLUDE) 

.PHONY: common all run clean

all: server

common: 
	cd ../common && $(MAKE) libaudiosync.a

server: $(OBJS) common
	gcc -o client $(OBJS) $(LDFLAGS) -laudiosync

run: all
	./client

clean:
	rm -f $(OBJS)
	rm -f client
	cd ../common && $(MAKE) clean
