
#pragma once
#include <SDL2/SDL_image.h>

#define TILE_SIZE 32

#define TILESET_WIDTH 12
#define TILESET_HEIGHT 30

typedef enum TILE_TYPE {
    TILE_NONE = -1,
    TILE_WALL = -2,

    // wall tiles
    TILE_WALL_N = 72,
    TILE_WALL_E = 84,
    TILE_WALL_S = 48,
    TILE_WALL_W = 108,

    TILE_WALL_NS = 60,
    TILE_WALL_EW = 96,

    TILE_WALL_NE = 87,
    TILE_WALL_NW = 75,
    TILE_WALL_SE = 51,
    TILE_WALL_SW = 63,

    TILE_WALL_NES = 54,
    TILE_WALL_NWS = 66,
    TILE_WALL_ESW = 90,
    TILE_WALL_ENW = 78,

    // rule words
    TILE_W_IS = 0,
    TILE_W_YOU = 6,
    TILE_W_STOP = 18,
    TILE_W_SICE = 30,
    TILE_W_NEAR = 99,
    TILE_W_1 = 11*12,
    TILE_W_2 = 12*12,
    TILE_W_3 = 13*12,
    TILE_W_4 = 14*12,
    TILE_W_5 = 15*12,
    TILE_W_6 = 16*12,
    TILE_W_7 = 17*12,
    TILE_W_8 = 18*12,
    TILE_W_0 = 19*12,

    // word objects
    TILE_W_THONK = 12,
    TILE_W_WALL = 15,
    TILE_W_DICE = 27,
    TILE_W_FLAG = 39,
    TILE_W_ON = 111,
    TILE_W_OFF = 114,
    TILE_W_CHECK = 3*12+9,

    TILE_W_CODE_RIGHT = 22*12+6,
    TILE_W_CODE_DOWN = 23*12+6,

    // objects
    TILE_THONK = 24,
    TILE_DICE = 36,
    TILE_FLAG = 42,
    TILE_ON = 123,
    TILE_OFF = 126,
    TILE_NO_CHECK = 28*12+6,
    TILE_CHECK = 28*12+9,

    // symbols
    TILE_S_CIRCLE_DOT = 20*12,
    TILE_S_D = 21*12,
    TILE_S_E = 22*12,
    TILE_S_FALSE = 23*12,
    TILE_S_BACK_L = 24*12,
    TILE_S_FILLED_TRIANGLE = 25*12,
    TILE_S_J = 26*12,
    TILE_S_X = 27*12,
    TILE_S_N = 28*12,
    TILE_S_G = 29*12,
    TILE_S_TRIANGLE = 11*12+6,
    TILE_S_HALF_SQUARE = 12*12+6,
    TILE_S_Z = 13*12+6,
    TILE_S_PLUS = 14*12+6,
    TILE_S_SQUARE = 15*12+6,
    TILE_S_TARGET = 16*12+6,
    TILE_S_SPLIT_CIRCLE = 17*12+6,
    TILE_S_BACK_R = 18*12+6,
    TILE_S_T = 19*12+6,
    TILE_S_PI = 20*12+6,
    TILE_S_SLASH = 21*12+6,
    TILE_S_I = 24*12+6,
    TILE_S_K = 25*12+6,
    TILE_S_F = 26*12+6,
    TILE_S_Y = 27*12+6,
} tile_type;

void init_tileset();
SDL_Texture *get_tileset();
SDL_Rect get_tile_rect(int idx);
