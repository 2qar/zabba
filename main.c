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
	while (playing) {
		while (SDL_PollEvent(&e))
			switch (e.type) {
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym) {
						case SDLK_ESCAPE:
							playing = 0;
							break;
						default:
							break;
					}
					switch (e.key.keysym.scancode) {
						case SDL_SCANCODE_W:
							player_pos.y -= 1;
							break;
						case SDL_SCANCODE_A:
							player_pos.x -= 1;
							break;
						case SDL_SCANCODE_S:
							player_pos.y += 1;
							break;
						case SDL_SCANCODE_D:
							player_pos.x += 1;
						default:
							break;
					}
					break;
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
