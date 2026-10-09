
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = my_ls
OBJS = main.o ls_core.o ls_format.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)