#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

// Define Window size
#define WINDOW_WIDTH  (640)
#define WINDOW_HEIGHT (480)

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	// Main loop flag
	uint8_t quit = 0; // false
	// Event handle
	SDL_Event e;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL initialize failed: %s\n", SDL_GetError());
		return -1;
	}

	// Create window
	window = SDL_CreateWindow("Load Image",
				  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				  WINDOW_WIDTH, WINDOW_HEIGHT,
				  SDL_WINDOW_SHOWN);
	if (NULL == window) {
		fprintf(stderr, "SDL create window failed: %s\n", SDL_GetError());
		goto error_create_window;
	}

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (NULL == renderer) {
		fprintf(stderr, "SDL create renderer failed: %s\n", SDL_GetError());
		goto error_create_renderer;
	}

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			default:
				break;
			}
		}

		// Clear screen
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		// Render red filled quad
		SDL_Rect fill_rect = {
			.x = WINDOW_WIDTH  / 4,
			.y = WINDOW_HEIGHT / 4,
			.w = WINDOW_WIDTH  / 2,
			.h = WINDOW_HEIGHT / 2,
		};
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(renderer, &fill_rect);

		// Render green outline quad
		SDL_Rect outline_rect = {
			.x = WINDOW_WIDTH  / 6,
			.y = WINDOW_HEIGHT / 6,
			.w = WINDOW_WIDTH  * 2 / 3,
			.h = WINDOW_HEIGHT * 2 / 3,
		};
		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderDrawRect(renderer, &outline_rect);

		// Draw blue horizontal line
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
		SDL_RenderDrawLine(renderer, 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT / 2);

		// Draw vertical line of yellow dots
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
		for (int i = 0; i < WINDOW_HEIGHT; i += 4)
			SDL_RenderDrawPoint(renderer, WINDOW_WIDTH / 2, i);

		// Update screen
		SDL_RenderPresent(renderer);
	}

	// Destroy render
	SDL_DestroyRenderer(renderer);
error_create_renderer:
	// Destroy window
	SDL_DestroyWindow(window);
error_create_window:
	// Quit SDL subsystems
	SDL_Quit();

	return 0;
}
