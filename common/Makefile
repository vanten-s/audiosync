OBJS = serialize.o fmt.o timer.o

NAME = libaudiosync.a

.PHONY: clean debug

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

debug: CFLAGS += -g
debug: $(NAME)

clean:
	rm -f $(OBJS) $(NAME)

