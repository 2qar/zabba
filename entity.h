#include <SDL2/SDL.h>

#define UP    1
#define DOWN  2
#define LEFT  4
#define RIGHT 8

typedef struct {
	SDL_Rect *pos;
	SDL_Rect *hitbox;
	float move_x, move_y;
	float move_speed;

	float _x_step, _y_step;
} entity_t;


void entity_move(entity_t *e, Uint32 delta, int dir);
void entity_set_pos(entity_t *e, int x, int y);
void entity_hitbox(entity_t *e, SDL_Rect *r);
SDL_bool entity_intersects(entity_t *e, entity_t *other);
SDL_bool entity_intersects_rect(entity_t *e, SDL_Rect *r);
void entity_uncollide(entity_t *e);
