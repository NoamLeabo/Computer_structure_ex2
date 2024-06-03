CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = cache
SRCS = main.c
OBJS = main.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f $(TARGET) $(OBJS)
