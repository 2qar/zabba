CC = cc
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = `pkg-config --libs sdl2` -lSDL2_image

default: main.o entity.o player.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o zabba $^

main.o: entity.o player.o
player.o: entity.o

clean:
	rm *.o zabba
