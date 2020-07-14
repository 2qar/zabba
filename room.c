#include "room.h"

int color_eq(SDL_Color c1, SDL_Color c2) {
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

void room_open_doors(room_t *r, SDL_Color c) {
	for (int i = 0; i < r->entities_len; ++i)
		if (r->entities[i].type == entity_type_door && color_eq(r->entities[i].color, c))
			r->entities[i].type = entity_type_door_opened;
}
