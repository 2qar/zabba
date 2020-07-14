#include <SDL2/SDL.h>

#ifndef ZABBA_ENTITY_H
#define ZABBA_ENTITY_H

#define UP    1
#define DOWN  2
#define LEFT  4
#define RIGHT 8

enum entity_type {
	entity_type_none,
	entity_type_enemy,
	entity_type_item, // TODO: make items drop in a random dir + have speed falloff (move_speed /= 1.0002f or smth)
	entity_type_key,
	entity_type_door,
	entity_type_door_opened,
};

typedef struct {
	enum entity_type type;

	SDL_Texture *texture;
	SDL_Color color;
	SDL_Rect pos;
	SDL_Rect hitbox;
	float move_x, move_y;
	float move_speed;

	float _x_step, _y_step;
} entity_t;


void entity_init(entity_t *e, const SDL_Rect *pos, const SDL_Rect *hitbox);
void entity_move(entity_t *e, Uint32 delta, int dir);
void entity_set_pos(entity_t *e, int x, int y);
void entity_hitbox(entity_t *e, SDL_Rect *r);
SDL_bool entity_intersects(entity_t *e, entity_t *other);
SDL_bool entity_intersects_rect(entity_t *e, SDL_Rect *r);
void entity_uncollide(entity_t *e);
void entity_disable(entity_t *e);
int entity_disabled(entity_t *e);

#endif
