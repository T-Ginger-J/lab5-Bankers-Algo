CC = gcc
CFLAGS = -pthread -Wall

all: banker

banker: banker.o
	$(CC) $(CFLAGS) -o banker banker.o

banker.o: bankers2.c
	$(CC) $(CFLAGS) -c bankers2.c

clean:
	rm -f banker banker.o

