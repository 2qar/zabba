CC = cc
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = `pkg-config --libs sdl2`

default: main.c
	$(CC) $(CFLAGS) main.c $(LDFLAGS) -o zabba
