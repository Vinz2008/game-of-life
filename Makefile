CC = gcc

CFLAGS = -c -g -Wall
#CFLAGS += -fsanitize=undefined
#CFLAGS += -fsanitize=address

SRCDIR = src

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

LDFLAGS = -lraylib -lm -pthread -ldl -lrt -lX11

#LDFLAGS += -fsanitize=undefined
#LDFLAGS += -fsanitize=address

all: game_of_life

game_of_life: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ $^

run: game_of_life
	./game_of_life

clean:
	rm -f $(OBJS) game_of_life