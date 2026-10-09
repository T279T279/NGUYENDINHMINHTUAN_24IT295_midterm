CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
TARGET = my_ls

SRCS = src/main.c src/options.c src/format.c src/core.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)