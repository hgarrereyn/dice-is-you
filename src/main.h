#include <SDL2/SDL.h>
#include <stdbool.h>

#ifndef MAIN
#define MAIN

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

SDL_Renderer *get_renderer();
void quit_game(void);
void set_freeze(bool);

#endif
