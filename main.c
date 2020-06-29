#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

#include "entity.h"

int main() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	SDL_Window *w = NULL;
	SDL_Renderer *r = NULL;
	if (SDL_CreateWindowAndRenderer(640, 480, 0, &w, &r) < 0) {
		SDL_Log("error initializing events: %s", SDL_GetError());
		return 1;
	}
	SDL_SetWindowTitle(w, "zabba");

	if (IMG_Init(IMG_INIT_PNG) < 0) {
		SDL_Log("error initializing SDL_image: %s", IMG_GetError());
		return 1;
	}

	SDL_Surface *player_surface = IMG_Load("bruh.png");
	if (!player_surface) {
		SDL_Log("error loading player image: %s", IMG_GetError());
		return 1;
	}

	IMG_Quit();

	SDL_Texture *player_texture = SDL_CreateTextureFromSurface(r, player_surface);
	if (!player_texture) {
		SDL_Log("error loading texture: %s", SDL_GetError());
		return 1;
	}

	SDL_Rect player_pos = { 40, 40, 32, 32 };
	entity_t player = {0};
	player.pos = &player_pos;
	player.hitbox = player.pos;
	player.move_speed = 0.2f;
	player.move_x = player.pos->x;
	player.move_y = player.pos->y;

	SDL_Event e;
	int playing = 1;

	SDL_Rect box = { 200, 200, 16, 16 };

	Uint32 old_time = SDL_GetTicks();
	while (playing) {
		Uint32 now = SDL_GetTicks();
		Uint32 delta = now - old_time;
		old_time = now;

		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_q)
						playing = 0;
					break;
			}
		}

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		int dir = 0;
		if (state[SDL_SCANCODE_W])
			dir |= UP;
		if (state[SDL_SCANCODE_S])
			dir |= DOWN;
		if (state[SDL_SCANCODE_A])
			dir |= LEFT;
		if (state[SDL_SCANCODE_D])
			dir |= RIGHT;
		entity_move(&player, delta, dir);
		if (entity_intersects_rect(&player, &box))
			entity_uncollide(&player);

		SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
		SDL_RenderClear(r);

		SDL_SetRenderDrawColor(r, 25, 25, 25, 255);
		SDL_RenderFillRect(r, &box);

		SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
		SDL_RenderDrawRect(r, &player_pos);
		SDL_RenderDrawRect(r, &box);

		SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
		SDL_RenderCopy(r, player_texture, NULL, &player_pos);

		SDL_RenderPresent(r);
	}

	SDL_FreeSurface(player_surface);
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);

	SDL_Quit();
	return 0;
}
