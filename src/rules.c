
#include "rules.h"
#include "entity.h"


bool _near_transform(entity entities[], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], entity *args[MAX_ARGS], bool *needs_move);
bool _transform(entity entities[], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], entity *args[MAX_ARGS], bool *needs_move);
bool _set_win(entity entities[], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], entity *args[MAX_ARGS], bool *needs_move);
bool _set_control(entity entities[], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], entity *args[MAX_ARGS], bool *needs_move);
bool _set_stop(entity entities[], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], entity *args[MAX_ARGS], bool *needs_move);
bool _check_code(entity entities[], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], entity *args[MAX_ARGS], bool *needs_move);
// bool _check_solved(entity entities[], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], entity *args[MAX_ARGS]);

pattern rules[] = {
    {
        .tiles={
            {.is_arg=true},
            {.is_arg=false, .type=TILE_W_NEAR},
            {.is_arg=true},
            {.is_arg=true},
            {.is_arg=false, .type=TILE_W_IS},
            {.is_arg=true}
        },
        .length=6,
        .fn=_near_transform
    },
    {
        .tiles={
            {.is_arg=true},
            {.is_arg=false, .type=TILE_W_IS},
            {.is_arg=true}
        },
        .length=3,
        .fn=_transform
    },
    {
        .tiles={
            {.is_arg=true},
            {.is_arg=false, .type=TILE_W_IS},
            {.is_arg=false, .type=TILE_W_SICE}
        },
        .length=3,
        .fn=_set_win
    },
    {
        .tiles={
            {.is_arg=true},
            {.is_arg=false, .type=TILE_W_IS},
            {.is_arg=false, .type=TILE_W_YOU}
        },
        .length=3,
        .fn=_set_control
    },
    {
        .tiles={
            {.is_arg=true},
            {.is_arg=false, .type=TILE_W_IS},
            {.is_arg=false, .type=TILE_W_STOP}
        },
        .length=3,
        .fn=_set_stop
    },
    {
        .tiles={
            {.is_arg=false, .type=TILE_W_CODE_RIGHT},
            {.is_arg=true},
            {.is_arg=true},
            {.is_arg=true},
            {.is_arg=true},
            {.is_arg=true},
        },
        .length=6,
        .fn=_check_code
    },
    {
        .tiles={
            {.is_arg=false, .type=TILE_W_CODE_DOWN},
            {.is_arg=true},
            {.is_arg=true},
            {.is_arg=true},
            {.is_arg=true},
            {.is_arg=true},
        },
        .length=6,
        .fn=_check_code
    },
    // {
    //     .tiles={
    //         {.is_arg=false, .type=TILE_NO_CHECK},
    //         {.is_arg=true},
    //         {.is_arg=true},
    //         {.is_arg=true},
    //         {.is_arg=true},
    //         {.is_arg=true},
    //     },
    //     .length=6,
    //     .fn=_check_solved
    // }
};
#define NUM_RULES 7

void _reset(entity entities[MAX_ENTITIES]) {
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        if (entities[i].active) {
            entities[i].is_active_text = false;
            entities[i].can_block = false;
            entities[i].can_control = false;
            entities[i].can_push = false;
            entities[i].can_win = false;
            entities[i].used_in_transform = false;

            // class based rules
            if (entities[i].is_text) {
                entities[i].can_push = true;
            }
        }
    }
}

int _get_word_reference(entity *t) {
    switch (t->type) {
        case TILE_W_THONK: return TILE_THONK;
        case TILE_W_DICE: return TILE_DICE;
        case TILE_W_FLAG: return TILE_FLAG;
        case TILE_W_WALL: return TILE_WALL;
        case TILE_W_ON: return TILE_ON;
        case TILE_W_OFF: return TILE_OFF;
        case TILE_W_CHECK: return TILE_CHECK;
        default: return -1;
    }
}

int _get_quantity(entity *t) {
    switch (t->type) {
        case TILE_W_0: return 0;
        case TILE_W_1: return 1;
        case TILE_W_2: return 2;
        case TILE_W_3: return 3;
        case TILE_W_4: return 4;
        case TILE_W_5: return 5;
        case TILE_W_6: return 6;
        case TILE_W_7: return 7;
        case TILE_W_8: return 8;
        default: return -1;
    }
}

// order: [212, 194, 189, 160, 150, 48, 247, 135, 1, 138, 192, 179, 119, 183, 55, 96, 171, 25, 61, 120, 148, 49, 5, 163, 18]
// unsigned char VALS = {1, 5, 18, 25, 48, 49, 55, 61, 96, 119, 120, 135, 138, 148, 150, 160, 163, 171, 179, 183, 189, 192, 194, 212, 247}

unsigned char _get_code_value(entity *t) {
    switch (t->type) {
        case TILE_S_BACK_L: return 1;
        case TILE_S_BACK_R: return 5;
        case TILE_S_CIRCLE_DOT: return 18;
        case TILE_S_D: return 25;
        case TILE_S_E: return 48;
        case TILE_S_F: return 49;
        case TILE_S_FALSE: return 55;
        case TILE_S_FILLED_TRIANGLE: return 61;
        case TILE_S_G: return 96;
        case TILE_S_HALF_SQUARE: return 119;
        case TILE_S_I: return 120;
        case TILE_S_J: return 135;
        case TILE_S_K: return 138;
        case TILE_S_N: return 148;
        case TILE_S_PI: return 150;
        case TILE_S_PLUS: return 160;
        case TILE_S_SLASH: return 163;
        case TILE_S_SPLIT_CIRCLE: return 171;
        case TILE_S_SQUARE: return 179;
        case TILE_S_T: return 183;
        case TILE_S_TARGET: return 189;
        case TILE_S_TRIANGLE: return 192;
        case TILE_S_X: return 194;
        case TILE_S_Y: return 212;
        case TILE_S_Z: return 247;
        default: return 0;
    }
}

bool _chain_contains(entity *c, tile_type type, bool set_active) {
    bool found = false;
    while (c) {
        if (c->type == type) {
            found = true;

            if (set_active) {
                c->is_active_text = true;
            }
        }
        c = c->next;
    }
    return found;
}

bool _near_transform(entity entities[], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], entity *args[MAX_ARGS], bool *needs_move) {
    entity *start = args[0];
    entity *quantity = args[1];
    entity *loc = args[2];
    entity *end = args[3];

    if (!start || !quantity || !loc || !end) return false;

    int start_ref = _get_word_reference(start);
    if (start_ref == -1) return false;

    int quantity_ref = _get_quantity(quantity);
    if (quantity_ref == -1) return false;
    // need to mark quantity text as it is an argument
    quantity->is_active_text = true;

    int loc_ref = _get_word_reference(loc);
    if (loc_ref == -1) return false;

    int end_ref = _get_word_reference(end);
    if (end_ref == -1) return false;

    // mark transform as used
    loc->used_in_transform = true;

    for (int i = 0; i < MAX_ENTITIES; ++i) {
        if (entities[i].active && entities[i].type == start_ref && !entities[i].is_transforming) {
            // check neighbors
            int count = 0;
            int x = entities[i].x;
            int y = entities[i].y;

            if (x > 0) {
                count += (int)_chain_contains(game_grid[y][x-1], loc_ref, false);
                if (y > 0) {
                    count += (int)_chain_contains(game_grid[y-1][x-1], loc_ref, false);
                }
                if (y < GAME_HEIGHT-1) {
                    count += (int)_chain_contains(game_grid[y+1][x-1], loc_ref, false);
                }
            }
            if (x < GAME_WIDTH-1) {
                count += (int)_chain_contains(game_grid[y][x+1], loc_ref, false);
                if (y > 0) {
                    count += (int)_chain_contains(game_grid[y-1][x+1], loc_ref, false);
                }
                if (y < GAME_HEIGHT-1) {
                    count += (int)_chain_contains(game_grid[y+1][x+1], loc_ref, false);
                }
            }
            if (y > 0) {
                count += (int)_chain_contains(game_grid[y-1][x], loc_ref, false);
            }
            if (y < GAME_HEIGHT-1) {
                count += (int)_chain_contains(game_grid[y+1][x], loc_ref, false);
            }

            if (count == quantity_ref) {
                // do transform
                entities[i].is_transforming = true;
                entities[i].next_type = end_ref;
            }
        }
    }

    return true;
}

bool _transform(entity entities[], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], entity *args[MAX_ARGS], bool *needs_move) {
    entity *start = args[0];

    if (start->used_in_transform) return false;

    bool valid = false;
    if (start) {
        int start_ref = _get_word_reference(start);
        if (start_ref != -1) {
            entity *end = args[1];
            if (end) {
                int end_ref = _get_word_reference(end);
                if (end_ref != -1) {
                    valid = true;

                    for (int i = 0; i < MAX_ENTITIES; ++i) {
                        if (entities[i].active && !entities[i].is_transforming && entities[i].type == start_ref) {
                            entities[i].is_transforming = true;
                            entities[i].next_type = end_ref;

                            *needs_move = true;
                        }
                    }
                }
            }
        }
    }

    return valid;
}

bool _set_win(entity entities[], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], entity *args[MAX_ARGS], bool *needs_move) {
    entity *t = args[0];

    bool valid = false;
    while (t) {
        int ref = _get_word_reference(t);
        if (ref != -1) { // invalid object reference
            valid = true;
            for (int i = 0; i < MAX_ENTITIES; ++i) {
                if (entities[i].active && entities[i].type == ref) {
                    entities[i].can_win = true;
                }
            }
        }
        t = t->next;
    }
    
    return valid;
}

bool _set_control(entity entities[], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], entity *args[MAX_ARGS], bool *needs_move) {
    entity *t = args[0];

    bool valid = false;
    while (t) {
        int ref = _get_word_reference(t);
        if (ref != -1) { // invalid object reference
            valid = true;
            for (int i = 0; i < MAX_ENTITIES; ++i) {
                if (entities[i].active && entities[i].type == ref) {
                    entities[i].can_control = true;
                }
            }
        }
        t = t->next;
    }
    
    return valid;
}

bool _set_stop(entity entities[], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], entity *args[MAX_ARGS], bool *needs_move) {
    entity *t = args[0];

    bool valid = false;
    while (t) {
        int ref = _get_word_reference(t);
        if (ref != -1) { // invalid object reference
            valid = true;
            for (int i = 0; i < MAX_ENTITIES; ++i) {
                if (entities[i].active && entities[i].type == ref) {
                    entities[i].can_block = true;
                }
            }
        }
        t = t->next;
    }
    
    return valid;
}

unsigned char _code(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e) {
    return ((a * 42) + (b * 1337) + c + (c ^ d) + (e*2));
}

bool _check_code(entity entities[], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], entity *args[MAX_ARGS], bool *needs_move) {
    unsigned char a = _get_code_value(args[0]);
    unsigned char b = _get_code_value(args[1]);
    unsigned char c = _get_code_value(args[2]);
    unsigned char d = _get_code_value(args[3]);
    unsigned char e = _get_code_value(args[4]);

    if (a == 0 || b == 0 || c == 0 || d == 0 || e ==0) {
        return false;
    }

    unsigned char val = _code(a,b,c,d,e);

    if (val == 0) {
        args[0]->is_active_text = true;
        args[1]->is_active_text = true;
        args[2]->is_active_text = true;
        args[3]->is_active_text = true;
        args[4]->is_active_text = true;
        return true;
    } else {
        return false;
    }
}

bool _check_rule(pattern *p, entity *o, int dx, int dy, entity entities[MAX_ENTITIES], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], bool set_active, bool *needs_move) {
    entity *args[MAX_ARGS];
    int arg_idx = 0;

    int x = o->x;
    int y = o->y;

    for (int i = 0; i < p->length; ++i) {
        if (x < 0 || y < 0 || x >= GAME_WIDTH || y >= GAME_HEIGHT) return false;

        pattern_element e = p->tiles[i];
        entity *curr = game_grid[y][x];

        // empty slot
        if (!curr) return false;

        if (e.is_arg) {
            // add to argument list
            args[arg_idx] = curr;
            arg_idx += 1;
        } else {
            // check if pattern matches
            if (!_chain_contains(curr, e.type, set_active)) {
                return false;
            }
        }

        x += dx;
        y += dy;
    }

    // try callback
    return p->fn(entities, game_grid, args, needs_move);
}

bool check_win_condition(entity entities[MAX_ENTITIES], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], entity **trigger) {
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        if (!entities[i].active) break;

        entity e = entities[i];
        entity *chain = game_grid[e.y][e.x];

        bool has_controlled = false;
        bool has_win = false;

        while (chain != NULL) {
            if (chain->can_control) {
                has_controlled = true;
                *trigger = chain;
            }
            if (chain->can_win) has_win = true;
            chain = chain->next;
        }

        if (has_controlled && has_win) {
            return true;
        }
    }

    return false;
}

bool update_rules(entity entities[MAX_ENTITIES], entity *game_grid[MAX_HEIGHT][MAX_WIDTH]) {
    _reset(entities);

    bool needs_move = false;

    for (int i = 0; i < NUM_RULES; ++i) {
        for (int j = 0; j < MAX_ENTITIES; ++j) {
            // horizontal
            if (_check_rule(&rules[i], &entities[j], 1, 0, entities, game_grid, false, &needs_move)) {
                _check_rule(&rules[i], &entities[j], 1, 0, entities, game_grid, true, &needs_move);
            }
            
            // vertical
            if (_check_rule(&rules[i], &entities[j], 0, 1, entities, game_grid, false, &needs_move)) {
                _check_rule(&rules[i], &entities[j], 0, 1, entities, game_grid, true, &needs_move);
            }
        }
    }

    return needs_move;
}
