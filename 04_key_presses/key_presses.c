#include <SDL2/SDL.h>
#include <stdio.h>

// Define Window size
#define WINDOW_WIDTH  (640)
#define WINDOW_HEIGHT (480)

// Key press surface constants
enum KeyPressSurfaces {
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL,
};

static const char *images[KEY_PRESS_SURFACE_TOTAL] = {
  "press.bmp", "up.bmp", "down.bmp", "left.bmp", "right.bmp"
};

int main(int argc, char *argv[])
{
  SDL_Window *window = NULL;
  SDL_Surface *surface = NULL;
  SDL_Surface *keypress_surfaces[KEY_PRESS_SURFACE_TOTAL];
  SDL_Surface *current_surface = NULL;
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

  // Get window surface
  surface = SDL_GetWindowSurface(window);

  // Fill the surface white
  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));

  // Load images
  for (uint8_t i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i) {
    keypress_surfaces[i] = SDL_LoadBMP(images[i]);
    if (NULL == keypress_surfaces[i]) {
      fprintf(stderr, "SDL load bmp image failed[name: %s]: %s\n", images[i], SDL_GetError());
      goto error_load_bmp;
    }
  }

  // Set default current surface
  current_surface = keypress_surfaces[KEY_PRESS_SURFACE_DEFAULT];

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
      case SDL_QUIT:
	quit = 1;
	break;
      case SDL_KEYDOWN: {
	switch (e.key.keysym.sym) {
	case SDLK_UP:
	  current_surface = keypress_surfaces[KEY_PRESS_SURFACE_UP];
	  break;
	case SDLK_DOWN:
	  current_surface = keypress_surfaces[KEY_PRESS_SURFACE_DOWN];
	  break;
	case SDLK_LEFT:
	  current_surface = keypress_surfaces[KEY_PRESS_SURFACE_LEFT];
	  break;
	case SDLK_RIGHT:
	  current_surface = keypress_surfaces[KEY_PRESS_SURFACE_RIGHT];
	  break;
	default:
	  current_surface = keypress_surfaces[KEY_PRESS_SURFACE_DEFAULT];
	  break;
	}
      } break;
      default:
	break;
      }
    }

    // Apply the image
    SDL_BlitSurface(current_surface, NULL, surface, NULL);

    // Update the surface
    SDL_UpdateWindowSurface(window);
  }

 error_load_bmp:
  // Free images surfaces
  for (uint16_t i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i) {
    if (keypress_surfaces[i]) {
      SDL_FreeSurface(keypress_surfaces[i]);
      keypress_surfaces[i] = NULL;
    }
  }
  // Destroy window
  SDL_DestroyWindow(window);
 error_create_window:
  // Quit SDL subsystems
  SDL_Quit();

  return 0;
}
