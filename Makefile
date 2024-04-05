CC=gcc
CFLAGS= -Wall -Wextra -g
DEPS = bankers.h
SRC = main.c customer.c
OBJ = $(SRC:.c=.o)

all: lab5

lab5: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.o lab5
