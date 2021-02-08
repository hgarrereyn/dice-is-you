
#include "main.h"
#include "tiles.h"

SDL_Texture *_tileset;


void init_tileset() {
    puts("init tileset");
    SDL_Surface *image = IMG_Load("assets/tiles.png");
    _tileset = SDL_CreateTextureFromSurface(get_renderer(), image);
}

SDL_Texture *get_tileset() {
    return _tileset;
}

SDL_Rect get_tile_rect(int idx) {
    int y = idx / TILESET_WIDTH;
    int x = idx % TILESET_WIDTH;

    return (SDL_Rect){x * TILE_SIZE, y * TILE_SIZE, 32, 32};
}
