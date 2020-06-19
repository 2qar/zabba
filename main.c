#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

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

	SDL_Surface *player = IMG_Load("bruh.png");
	if (!player) {
		SDL_Log("error loading player image: %s", IMG_GetError());
		return 1;
	}

	IMG_Quit();

	SDL_Texture *player_texture = SDL_CreateTextureFromSurface(r, player);
	if (!player_texture) {
		SDL_Log("error loading texture: %s", SDL_GetError());
		return 1;
	}

	SDL_Rect player_pos = { 40, 40, 32, 32 };
	SDL_Event e;
	int playing = 1;
	float x = player_pos.x, y = player_pos.y;
	const float move_speed = 0.2f;

	Uint32 old_time = SDL_GetTicks();
	while (playing) {
		Uint32 now = SDL_GetTicks();
		Uint32 delta = now - old_time;
		old_time = now;

		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_ESCAPE) {
						playing = 0;
						break;
					}
			}
		}

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_W]) {
			y -= move_speed * delta;
			player_pos.y = (int)y;
		}
		if (state[SDL_SCANCODE_S]) {
			y += move_speed * delta;
			player_pos.y = (int)y;
		}
		if (state[SDL_SCANCODE_A]) {
			x -= move_speed * delta;
			player_pos.x = (int)x;
		}
		if (state[SDL_SCANCODE_D]) {
			x += move_speed * delta;
			player_pos.x = (int)x;
		}

		SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
		SDL_RenderClear(r);
		SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
		SDL_RenderCopy(r, player_texture, NULL, &player_pos);
		SDL_RenderPresent(r);
	}

	SDL_FreeSurface(player);
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);

	SDL_Quit();
	return 0;
}
