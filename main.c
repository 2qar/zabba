#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>

#include "entity.h"
#include "player.h"

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
	SDL_Texture *enemy_texture = SDL_CreateTextureFromSurface(r, player_surface);
	if (!enemy_texture) {
		SDL_Log("error creating enemy texture: %s", SDL_GetError());
		return 1;
	}
	SDL_SetTextureColorMod(enemy_texture, 255, 0, 0);

	/* player junk */
	SDL_Rect player_pos = { 100, 40, 32, 32 };
	SDL_Rect player_hitbox = { 3, 18, 25, 12 };
	entity_t player_e = {0};
	entity_init(&player_e, &player_pos, &player_hitbox);

	player_t player = {0};
	player.e = &player_e;
	player.walk_speed = 0.2f;
	player.e->move_speed = player.walk_speed;
	player.roll_speed = 0.4f;
	player.roll_time_ms = 200;

	/* enemy!!!! */
	SDL_Rect enemy_pos = { 200, 32, 32, 32 };
	SDL_Rect enemy_hitbox = { 0, 0, 32, 32 };
	entity_t enemy = {0};
	entity_init(&enemy, &enemy_pos, &enemy_hitbox);
	enemy.type = entity_type_enemy;

	/* debug hitbox info */
	SDL_Rect hb;

	/* room junk */
	const int room_width = 15;
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
	for (int i = 0; i < walls_len; ++i) {
		walls[i].w = walls[i].h = wall_size;
		walls[i].x += wall_size * 2;
	}

	SDL_Rect room_bg = { 0, 0, room_width * wall_size, room_height * wall_size };
	room_bg.x += wall_size * 2;

	/* pseudo-room junk */
	int rooms[][3] = {
		{ RIGHT, LEFT | DOWN , 0    },
		{ 0    , UP   | DOWN , 0    },
		{ 0    , UP   | RIGHT, LEFT },
	};
	int room_y = 1;
	int room_x = 1;

	SDL_Event e;
	int playing = 1;
	int show_hitboxes = 0;
	Uint32 old_time = SDL_GetTicks();
	while (playing) {
		Uint32 now = SDL_GetTicks();
		Uint32 delta = now - old_time;
		old_time = now;

		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_KEYDOWN:
					/* icky indentation!!!! */
					switch (e.key.keysym.sym) {
						case SDLK_ESCAPE:
						case SDLK_q:
							playing = 0;
							break;
						case SDLK_TAB:
							show_hitboxes = !show_hitboxes;
							break;
						case SDLK_SPACE:
							player_roll(&player);
							break;
					}
					break;
			}
		}

		player_move(&player, delta);

		SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
		SDL_RenderClear(r);

		SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
		SDL_RenderFillRect(r, &room_bg);

		for (int i = 0; i < walls_len; ++i) {
			SDL_SetRenderDrawColor(r, 25, 25, 25, 255);
			/* FIXME: this is fuckin dumb */
			if ((rooms[room_y][room_x] & UP) && i == room_width / 2)
				continue;
			if ((rooms[room_y][room_x] & DOWN) && i == room_width / 2 + room_width)
				continue;
			if ((rooms[room_y][room_x] & LEFT) && i == offset + room_height / 2)
				continue;
			if ((rooms[room_y][room_x] & RIGHT) && i == offset + room_height + room_height / 2)
				continue;

			SDL_RenderFillRect(r, &walls[i]);
			if (entity_intersects_rect(player.e, &walls[i]))
				entity_uncollide(player.e);

			if (show_hitboxes) {
				SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
				SDL_RenderDrawRect(r, &walls[i]);
			}
		}

		if (entity_intersects(player.e, &enemy)) {
			if (player.rolling) {
				enemy.hitbox->w = enemy.hitbox->h = 0;
				enemy.pos->x = enemy.pos->y = -1;
			} else {
				player.e->pos->w = player.e->pos->h = 0;
				entity_set_pos(player.e, walls[0].x + 1, walls[0].y + 1);
				/* TODO: show some text or maybe a reset prompt */
				SDL_Log("you died :(");
			}
		}

		if (enemy_pos.x > 0)
			SDL_RenderCopy(r, enemy_texture, NULL, &enemy_pos);

		if (show_hitboxes) {
			SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
			entity_hitbox(&enemy, &hb);
			SDL_RenderDrawRect(r, &hb);
			entity_hitbox(player.e, &hb);
			SDL_RenderDrawRect(r, &hb);
		}

		if (player.rolling)
			SDL_SetTextureColorMod(player_texture, 200, 200, 200);
		else
			SDL_SetTextureColorMod(player_texture, 255, 255, 255-50);
		SDL_RenderCopy(r, player_texture, NULL, &player_pos);

		SDL_RenderPresent(r);

		/* flip flop rooms */
		if (player_pos.x < walls[0].x) {
			room_x -= 1;
			entity_set_pos(player.e, walls[room_width-1].x, player.e->pos->y);
		}
		else if (player_pos.x > walls[room_width-1].x) {
			room_x += 1;
			entity_set_pos(player.e, walls[0].x, player.e->pos->y);
		}
		else if (player_pos.y < walls[0].y) {
			room_y -= 1;
			entity_set_pos(player.e, player.e->pos->x, walls[walls_len-1].y);
		}
		else if (player_pos.y > walls[room_width].y) {
			room_y += 1;
			entity_set_pos(player.e, player.e->pos->x, walls[offset].y);
		}
	}

	free(walls);

	SDL_FreeSurface(player_surface);
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);

	SDL_Quit();
	return 0;
}
