#include <SDL2/SDL.h>

#include "entity.h"

typedef struct {
	entity_t *e;
	int move_dir;
	float walk_speed, roll_speed;
	int rolling;
	Uint32 roll_time_ms;

	Uint32 _roll_timeout;
} player_t;

void player_roll(player_t *p);
void player_move(player_t *p, Uint32 delta);
