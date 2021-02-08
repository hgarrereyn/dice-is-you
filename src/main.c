#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "main.h"
#include "game.h"
#include "tiles.h"
#include "levels.h"

void handle_events(void);
void quit(void);

SDL_Window *window;
SDL_Renderer *renderer;

bool running = false;
bool frozen = false;

clock_t last_frame;


bool init(void) {
    bool success = true;
    window = NULL;
    renderer = NULL;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not be initiliazed. SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    window = SDL_CreateWindow("Dice is you", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(!window){
        printf("SDL_Window could not be initialized. SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else{
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);    
    }

    init_tileset();
    init_game();

    load_cookie();

    last_frame = clock();

    running = true;
    return success;
}

void main_loop(void) {
    handle_events();

    if(frozen)
        return;

    clock_t current = clock();
    unsigned long delta = current - last_frame;
    last_frame = current;

    update_game(delta);
    render_game(delta);

    SDL_RenderPresent(renderer);
}

int main(int argc, char* args[])
{
    if(!init())
        return -1;
    else{

        #ifdef __EMSCRIPTEN__
            emscripten_set_main_loop(main_loop, 0, 1);
        #endif

        #ifndef __EMSCRIPTEN__
            while(running)
                main_loop();
        #endif
    }
    
    quit_game();
    return 0;
}

void quit_game(void) {
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_Quit();

    #ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
    #endif
}

void set_freeze(bool b) {
    frozen = b;
}

SDL_Renderer* get_renderer() { 
    return renderer; 
}
