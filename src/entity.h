
#pragma once
#include <stdbool.h>
#include "tiles.h"


typedef struct _entity {
    bool active;
    tile_type type;
    int x;
    int y;

    // linked list
    struct _entity *next;
    struct _entity *prev;

    // movement
    bool is_moving;
    int dx;
    int dy;

    // transformation
    bool is_transforming;
    tile_type next_type;

    // fixed attributes
    bool is_text;

    // mutable attributes
    bool is_active_text;
    bool can_push; // can be pushed
    bool can_control; // controllable like a player
    bool can_block; // prevents movement
    bool can_win; // win state like a flag

    // rule collisions
    bool used_in_transform; // true if this is part of a larger rule and shouldn't be interpreted again
} entity;
