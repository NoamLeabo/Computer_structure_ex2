CC=gcc
CFLAGS=-O3 -Wall
LIBS=

SRCS=main.c utilities.c fmm.c
OBJS=$(SRCS:.c=.o)
EXEC=fmm

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

main.o: main.c fmm.h
	$(CC) $(CFLAGS) -c $< -o $@

utilities.o: utilities.c fmm.h
	$(CC) $(CFLAGS) -c $< -o $@

fmm.o: fmm.c fmm.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
