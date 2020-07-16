CC = cc
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = `pkg-config --libs sdl2` -lSDL2_image -lSDL2_ttf

default: main.o entity.o player.o room.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o zabba $^

main.o: entity.o player.o room.o
player.o: entity.o
room.o: entity.o

clean:
	rm *.o zabba
