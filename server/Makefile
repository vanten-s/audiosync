CC = gcc

OBJS = main.o

server: $(OBJS)
	gcc -o server $(OBJS)

all: server

run: all
	./server

clean:
	rm -f $(OBJS)
	rm -f server
