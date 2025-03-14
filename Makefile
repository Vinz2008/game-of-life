CC = gcc
CXX = g++ # for profiler
LINKER = $(CC)

TRACY ?= FALSE

CFLAGS = -c -g -Wall

#CFLAGS += -fsanitize=undefined
#CFLAGS += -fsanitize=address

SRCDIR = src

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

ifneq ($(TRACY), FALSE)
	LINKER = $(CXX)
	OBJS += external/tracy/public/TracyClient.o
	CFLAGS += -DTRACY_ENABLE=1 -DTRACY_NO_EXIT=1 -Iexternal/tracy/public/tracy
endif

LDFLAGS = -lraylib -lm -pthread -ldl -lrt -lX11

#LDFLAGS += -fsanitize=undefined
#LDFLAGS += -fsanitize=address

all: game_of_life

game_of_life: $(OBJS)
	$(LINKER) -o $@ $^ $(LDFLAGS)

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ $^

external/tracy/public/TracyClient.o: external/tracy/public/TracyClient.cpp
	$(CXX) $(CFLAGS) -o $@ $^

run: game_of_life
	./game_of_life

clean:
	rm -f $(OBJS) game_of_life

