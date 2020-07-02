#include "player.h"

void player_roll(player_t *p) {
	if (!p->rolling && p->move_dir != 0) {
		p->rolling = 1;
		p->e->move_speed = p->roll_speed;
		p->_roll_timeout = SDL_GetTicks() + p->roll_time_ms;
	}
}

void player_move(player_t *p, Uint32 delta) {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (!p->rolling) {
		p->move_dir = 0;
		if (state[SDL_SCANCODE_W])
			p->move_dir |= UP;
		if (state[SDL_SCANCODE_S])
			p->move_dir |= DOWN;
		if (state[SDL_SCANCODE_A])
			p->move_dir |= LEFT;
		if (state[SDL_SCANCODE_D])
			p->move_dir |= RIGHT;
		entity_move(p->e, delta, p->move_dir);
	} else if (!SDL_TICKS_PASSED(SDL_GetTicks(), p->_roll_timeout)){
		entity_move(p->e, delta, p->move_dir);
	} else {
		p->rolling = 0;
		p->e->move_speed = p->walk_speed;
	}
}
