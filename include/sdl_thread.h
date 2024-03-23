#ifndef SDL_THREAD_H
#define SDL_THREAD_H

//#include <SDL2/SDL.h>
#include <vector>
#include <array>

extern int width;
extern int height;

extern std::vector<std::vector<std::array<int, 3>>> pixels;

void handleSDL(int argc);

#endif // SDL_THREAD_H
