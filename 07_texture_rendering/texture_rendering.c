#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

// Define Window size
#define WINDOW_WIDTH  (640)
#define WINDOW_HEIGHT (480)

SDL_Texture *load_texture(const char *path, const SDL_Renderer *base)
{
	// The final optimized image
	SDL_Texture *texture = NULL;

	// Load image at specified path
	SDL_Surface *loaded = IMG_Load(path);
	if (NULL == loaded)
		fprintf(stderr, "Unable to load image: %s! SDL Error: %s\n", path, SDL_GetError());
	else {
		texture = SDL_CreateTextureFromSurface((SDL_Renderer *)base, loaded);
		if (NULL == texture)
			fprintf(stderr, "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError());

		// Get rid of old loaded surface
		SDL_FreeSurface(loaded);
	}

	return texture;
}

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *texture = NULL;
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

	// Initialize PNG loading
	int flags = IMG_INIT_PNG;
	if (!(IMG_Init(flags) & flags)) {
		fprintf(stderr, "SDL_image could not initialize! SDL Error: %s\n", IMG_GetError());
		goto error_image_init;
	}

	// Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (NULL == renderer) {
		fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		goto error_create_renderer;
	}


	// Initialize renderer color
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// Load image
	texture = load_texture("./texture.png", renderer);
	if (NULL == texture) {
		fprintf(stderr, "SDL load bmp image failed: %s\n", SDL_GetError());
		goto error_load_bmp;
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
		SDL_RenderClear(renderer);

		// Render texture to screen
		SDL_RenderCopy(renderer, texture, NULL, NULL);

		// Update screen
		SDL_RenderPresent(renderer);

	}

	SDL_DestroyTexture(texture);
	texture = NULL;
error_load_bmp:
	// Destroy render
	SDL_DestroyRenderer(renderer);
error_create_renderer:
error_image_init:
	// Destroy window
	SDL_DestroyWindow(window);
error_create_window:
	// Quit SDL subsystems
	SDL_Quit();

	return 0;
}
