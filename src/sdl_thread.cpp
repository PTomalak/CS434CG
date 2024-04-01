#include "sdl_thread.h"

extern int refresh;

// Function to handle SDL
void handleSDL(int argc) {
  if (argc <= 3) {
    return;
  }
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  // Initialize SDL
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("Raytracing", 4000, SDL_WINDOWPOS_UNDEFINED, width,
                            height, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // Main loop
  bool quit = false;
  SDL_Event e;
  while (!quit) {
    // Handle events
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      } else if (e.type == SDL_KEYDOWN &&
                 (e.key.keysym.sym == SDLK_ESCAPE ||
                  e.key.keysym.sym == SDLK_c || e.key.keysym.sym == SDLK_q)) {
        quit = true; // Quit if ESC key is pressed
      } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r) {
        refresh = 1;
        quit = true;
      }
    }

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw pixels
    for (int x = 0; x < width; ++x) {
      for (int y = 0; y < height; ++y) {
        SDL_SetRenderDrawColor(renderer, pixels[x][y][0], pixels[x][y][1],
                               pixels[x][y][2], 255);
        SDL_RenderDrawPoint(renderer, x, y);
      }
    }

    // Render screen
    SDL_RenderPresent(renderer);
  }

  // Clean up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
