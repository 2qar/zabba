#include <SDL2/SDL.h>

#include "entity.h"

typedef struct {
	int doors;
	int entities_len;
	entity_t *entities;
} room_t;

void room_open_doors(room_t *, SDL_Color c);
