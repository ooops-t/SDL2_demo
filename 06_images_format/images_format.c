#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

// Define Window size
#define WINDOW_WIDTH  (640)
#define WINDOW_HEIGHT (480)

SDL_Surface *load_surface(const char *path, const SDL_Surface *base)
{
	// The final optimized image
	SDL_Surface *optimized = NULL;

	// Load image at specified path
	SDL_Surface *loaded = IMG_Load(path);
	if (NULL == loaded)
		fprintf(stderr, "Unable to load image: %s! SDL Error: %s\n", path, SDL_GetError());
	else {
		optimized = SDL_ConvertSurface(loaded, base->format, 0);
		if (NULL == optimized)
			fprintf(stderr, "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError());

		// Get rid of old loaded surface
		SDL_FreeSurface(loaded);
	}

	return optimized;
}

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Surface *surface = NULL;
	SDL_Surface *image = NULL;
	// Main loop flag
	uint8_t quit = 0; // false
	// Event handle
	SDL_Event e;
	// Apply the image stretched
	SDL_Rect stretch_rect;
	stretch_rect.x = 0;
	stretch_rect.y = 0;
	stretch_rect.w = WINDOW_WIDTH;
	stretch_rect.h = WINDOW_HEIGHT;

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

	// Get window surface
	surface = SDL_GetWindowSurface(window);

	// Fill the surface white
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));

	// Load image
	image = load_surface("./loaded.png", surface);
	if (NULL == image) {
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

		// Apply the image
		SDL_BlitScaled(image, NULL, surface, &stretch_rect);

		// Update the surface
		SDL_UpdateWindowSurface(window);
	}

	SDL_FreeSurface(image);
	image = NULL;
error_load_bmp:
error_image_init:
	// Destroy window
	SDL_DestroyWindow(window);
error_create_window:
	// Quit SDL subsystems
	SDL_Quit();

	return 0;
}
