#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>

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

	const int room_width = 16;
	const int room_height = 14;
	const int wall_size = 32;

	/* TODO: center the room in the middle of the screen */
	const int walls_len = (room_width * 2 + room_height * 2);
	SDL_Rect *walls = malloc(sizeof(SDL_Rect) * walls_len);
	/* top + bottom walls */
	for (int i = 0; i < room_width; ++i) {
		walls[i].x = i * wall_size;
		walls[i].y = 0;

		walls[i + room_width].x = i * wall_size;
		walls[i + room_width].y = (room_height * wall_size) - wall_size;
	}

	/* side walls */
	int offset = room_width * 2;
	for (int i = offset; i < offset + room_height; ++i) {
		walls[i].x = 0;
		walls[i].y = (i - offset)* wall_size;

		walls[i + room_height].x = (room_width * wall_size) - wall_size;
		walls[i + room_height].y = (i - offset) * wall_size;
	}

	/* give each wall its size */
	for (int i = 0; i < walls_len; ++i)
		walls[i].w = walls[i].h = wall_size;

	SDL_Rect room_bg = { 0, 0, room_width * wall_size, room_height * wall_size };

	SDL_Event e;
	int playing = 1;
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

		SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
		SDL_RenderClear(r);

		SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
		SDL_RenderFillRect(r, &room_bg);

		SDL_SetRenderDrawColor(r, 25, 25, 25, 255);
		for (int i = 0; i < walls_len; ++i) {
			SDL_RenderFillRect(r, &walls[i]);
			if (entity_intersects_rect(&player, &walls[i]))
				entity_uncollide(&player);
		}

		SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
		SDL_RenderDrawRect(r, &player_pos);
		for (int i = 0; i < walls_len; ++i)
			SDL_RenderDrawRect(r, &walls[i]);

		SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
		SDL_RenderCopy(r, player_texture, NULL, &player_pos);

		SDL_RenderPresent(r);
	}

	free(walls);

	SDL_FreeSurface(player_surface);
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);

	SDL_Quit();
	return 0;
}
