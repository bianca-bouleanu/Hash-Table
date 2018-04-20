# Tema 1 - Hash table - Makefile Linux
# Oana-Bianca Bouleanu, 333CA

CC=gcc
CFLAGS = -Wall -Wextra -g

all: build

build:
	$(CC) $(CFLAGS) functions.c tema1.c -lhash -L. -o tema1

tema1.o: tema1.c
	$(CC) $(CFLAGS) -c tema1.c

clean:
	rm -f tema1.o tema1
