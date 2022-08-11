#include <SDL2/SDL.h>
#include <stdio.h>

// Define Window size
#define WINDOW_WIDTH  (640)
#define WINDOW_HEIGHT (480)

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Surface *surface = NULL;
	SDL_Surface *image = NULL;

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

	// Get window surface
	surface = SDL_GetWindowSurface(window);

	// Fill the surface white
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));

	// Load image
	image = SDL_LoadBMP("./hello_world.bmp");
	if (NULL == image) {
		fprintf(stderr, "SDL load bmp image failed: %s\n", SDL_GetError());
		goto error_load_bmp;
	}

	// Apply the image
	SDL_BlitSurface(image, NULL, surface, NULL);

	// Update the surface
	SDL_UpdateWindowSurface(window);

	// Wait two seconds
	SDL_Delay(2000);

	SDL_FreeSurface(image);
	image = NULL;
error_load_bmp:
	// Destroy window
	SDL_DestroyWindow(window);
error_create_window:
	// Quit SDL subsystems
	SDL_Quit();

	return 0;
}
