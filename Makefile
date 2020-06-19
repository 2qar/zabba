CC = cc
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = `pkg-config --libs sdl2` -lSDL2_image

default: main.c
	$(CC) $(CFLAGS) main.c $(LDFLAGS) -o zabba
