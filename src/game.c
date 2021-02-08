
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "tiles.h"
#include "game.h"
#include "entity.h"
#include "rules.h"
#include "memory.h"
#include "levels.h"


// constants
#define TILE_ANIMATION_DELAY 150 // milliseconds
#define MOVE_DELAY 100 // milliseconds

#define LEVEL_START_TIME 500 // milliseconds
#define LEVEL_TRANSITION_TIME 300 // milliseconds
#define LEVEL_END_TIME 500 // milliseconds

// entities
entity entities[MAX_ENTITIES]; // entity buffer backing
entity *game_grid[MAX_HEIGHT][MAX_WIDTH]; // grid based entity lookup

// game state
int _game_frame = 0;
unsigned long _counter_tile_animation = 0;

unsigned long _counter_move = 0;
bool has_move = false;
bool ghost_move = false; // true if this is a rule-initiated move
bool is_replay = false; // true if this move is a memory replay
int pre_dx = 0;
int pre_dy = 0;


// Level end (fade out) || reshape animation || (fade in) level start


// level transition animation
unsigned long _counter_level_transition = 0;
entity *trigger_entity = 0;
int next_level = 0;
bool is_level_complete = false; // fade out
bool is_level_switching = false; // reshape
bool is_level_starting = false; // fade in

// level state
int GAME_WIDTH = 1;
int GAME_HEIGHT = 1;

// def
void _try_move(int,int);



// utility
SDL_Rect _get_tile_pos_custom(int x, int y, int width, int height) {
    int max_width = (SCREEN_WIDTH - (2 * GAME_BORDER)) / width;
    int max_height = (SCREEN_HEIGHT - (2 * GAME_BORDER)) / height;

    int tile_size = max_height < max_width ? max_height : max_width;

    int origin_x = (SCREEN_WIDTH / 2) - ((width * tile_size) / 2);
    int origin_y = (SCREEN_HEIGHT / 2) - ((height * tile_size) / 2);

    return (SDL_Rect){
        origin_x + (x * tile_size),
        origin_y + (y * tile_size),
        tile_size,
        tile_size
    };
}

SDL_Rect _get_tile_pos(int x, int y) {
    return _get_tile_pos_custom(x,y,GAME_WIDTH,GAME_HEIGHT);
}

int _get_interpolation(int start, int end, float t) {
    float sqt = t * t;
    float f = sqt / (2.0f * (sqt - t) + 1.0f);
    return ((end - start) * f) + start;
}

SDL_Rect _get_scaled(SDL_Rect *orig, float t) {
    // 0: full size
    // 1: zero size
    int offset = _get_interpolation(0, orig->w / 2, t);
    int sz = _get_interpolation(orig->w, 0, t);

    return (SDL_Rect){
        orig->x + offset,
        orig->y + offset,
        sz,
        sz
    };
}

SDL_Rect _get_window_rect(int width, int height) {
    SDL_Rect start = _get_tile_pos_custom(0,0,width,height);
    SDL_Rect end = _get_tile_pos_custom(width, height, width, height);

    SDL_Rect inside = {
        start.x,
        start.y,
        end.x - start.x,
        end.y - start.y
    };

    return inside;
}

SDL_Rect _interpolate_rect(SDL_Rect target1, SDL_Rect target2, float completion) {
    SDL_Rect merged = (SDL_Rect){
        _get_interpolation(target1.x, target2.x, completion),
        _get_interpolation(target1.y, target2.y, completion),
        _get_interpolation(target1.w, target2.w, completion),
        _get_interpolation(target1.h, target2.h, completion)
    };

    return merged;
}

void _draw_background() {
    SDL_Rect inside = _get_window_rect(GAME_WIDTH, GAME_HEIGHT);

    // dark gray
    SDL_SetRenderDrawColor(get_renderer(), 44, 62, 80, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(get_renderer(), &inside);
}

void _draw_fade_background(float completion) {
    SDL_Rect inside = _get_window_rect(GAME_WIDTH, GAME_HEIGHT);

    unsigned char alpha = (unsigned char)(completion * 255);

    // dark gray
    SDL_SetRenderDrawBlendMode(get_renderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(get_renderer(), 44, 62, 80, alpha);
    SDL_RenderFillRect(get_renderer(), &inside);
}

void _draw_interpolate_background(float completion, int new_width, int new_height) {
    SDL_Rect inside1 = _get_window_rect(GAME_WIDTH, GAME_HEIGHT);
    SDL_Rect inside2 = _get_window_rect(new_width, new_height);

    SDL_Rect inside = _interpolate_rect(inside1, inside2, completion);

    // dark gray
    SDL_SetRenderDrawColor(get_renderer(), 44, 62, 80, 255);
    SDL_RenderFillRect(get_renderer(), &inside);
}

void _render_tile(int base_idx, int frame, int x, int y) {
    SDL_Rect target = _get_tile_pos(x, y);
    SDL_Rect src = get_tile_rect(base_idx + frame);
    SDL_RenderCopy(get_renderer(), get_tileset(), &src, &target);
}

void _render_tile_interpolated(int base_idx, int frame, int x, int y, int dx, int dy, float alpha) {
    SDL_Rect start = _get_tile_pos(x, y);
    SDL_Rect end = _get_tile_pos(x+dx, y+dy);

    start.x = _get_interpolation(start.x, end.x, alpha);
    start.y = _get_interpolation(start.y, end.y, alpha);

    SDL_Rect src = get_tile_rect(base_idx + frame);
    SDL_RenderCopy(get_renderer(), get_tileset(), &src, &start);
}

void _render_tile_interpolated_transform_raw(int base_idx1, int base_idx2, int frame, int x, int y, int dx, int dy, float alpha, int width, int height) {
    SDL_Rect start = _get_tile_pos_custom(x, y, width, height);
    SDL_Rect end = _get_tile_pos_custom(x+dx, y+dy, width, height);

    start.x = _get_interpolation(start.x, end.x, alpha);
    start.y = _get_interpolation(start.y, end.y, alpha);

    SDL_Rect src;
    SDL_Rect target;
    if (alpha < 0.5) {
        src = get_tile_rect(base_idx1 + frame);
        target = _get_scaled(&start, alpha * 1.5);
    } else {
        src = get_tile_rect(base_idx2 + frame);
        target = _get_scaled(&start, (1 - alpha) * 1.5);
    }

    SDL_RenderCopy(get_renderer(), get_tileset(), &src, &target);
}

void _render_tile_interpolated_transform(int base_idx1, int base_idx2, int frame, int x, int y, int dx, int dy, float alpha) {
    _render_tile_interpolated_transform_raw(base_idx1, base_idx2, frame, x, y, dx, dy, alpha, GAME_WIDTH, GAME_HEIGHT);
}

void _render_tile_win_animation(int base_idx1, int frame, int x, int y, int target_level, float completion) {
    // completion: 0 (normal) -> 1 (full size)
    level next_level = levels[target_level];
    int tx = next_level.player_x;
    int ty = next_level.player_y;

    SDL_Rect target1 = _get_tile_pos(x, y);
    SDL_Rect target2 = _get_tile_pos_custom(tx, ty, next_level.width, next_level.height);

    SDL_Rect merged = _interpolate_rect(target1, target2, completion);

    SDL_Rect src = get_tile_rect(base_idx1 + frame);
    SDL_RenderCopy(get_renderer(), get_tileset(), &src, &merged);
}

bool _has_wall(int x, int y) {
    entity *t = game_grid[y][x];
    while (t) {
        if (t->type == TILE_WALL) {
            return true;
        }
        t = t->next;
    }
    return false;
}

void _render_wall(int x, int y) {
    int north = (y == 0 || _has_wall(x,y-1));
    int south = (y == GAME_HEIGHT-1 || _has_wall(x,y+1));
    int west = (x == 0 || _has_wall(x-1, y));
    int east = (x == GAME_WIDTH-1 || _has_wall(x+1, y));

    int v = (north << 3) + (east << 2) + (south << 1) + west;
    int base_idx = 0;
    switch (v) {
        case 0b1000: base_idx = TILE_WALL_N; break;
        case 0b0100: base_idx = TILE_WALL_E; break;
        case 0b0010: base_idx = TILE_WALL_S; break;
        case 0b0001: base_idx = TILE_WALL_W; break;

        case 0b1010: base_idx = TILE_WALL_NS; break;
        case 0b0101: base_idx = TILE_WALL_EW; break;
        
        case 0b1100: base_idx = TILE_WALL_NE; break;
        case 0b1001: base_idx = TILE_WALL_NW; break;
        case 0b0110: base_idx = TILE_WALL_SE; break;
        case 0b0011: base_idx = TILE_WALL_SW; break;

        case 0b1110: base_idx = TILE_WALL_NES; break;
        case 0b1011: base_idx = TILE_WALL_NWS; break;
        case 0b1101: base_idx = TILE_WALL_ENW; break;
        case 0b0111: base_idx = TILE_WALL_ESW; break;

        default: return;
    }

    _render_tile(base_idx, _game_frame, x, y);
}

void _render_entity(entity *t) {
    if (t->type == TILE_WALL) {
        _render_wall(t->x, t->y);
        return;
    }

    int base_idx = t->type;
    if (t->is_active_text) base_idx += 3;

    if (t->is_moving || t->is_transforming) {
        if (!t->is_transforming)
            _render_tile_interpolated(base_idx, _game_frame, t->x, t->y, t->dx, t->dy, (_counter_move / (float)MOVE_DELAY));
        else
            _render_tile_interpolated_transform(
                t->type, t->next_type, _game_frame, t->x, t->y, t->dx, t->dy, (_counter_move / (float)MOVE_DELAY));
    } else {
        // static tile
        _render_tile(base_idx, _game_frame, t->x, t->y);
    }
}

void render_game(unsigned long delta) {
    // light gray
    SDL_SetRenderDrawColor(get_renderer(), 52, 73, 94, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(get_renderer());

    if (!is_level_switching) {
        _draw_background();

        for (int i = 0; i < MAX_ENTITIES; ++i) {
            if (entities[i].active) {
                _render_entity(&entities[i]);
            }
        }
    }

    if (is_level_complete) {
        float completion = ((float)_counter_level_transition) / (float)LEVEL_END_TIME;

        // Fade out.
        _draw_fade_background(completion);

        // Interpolate tile position.
        _render_tile_win_animation(
            trigger_entity->type, _game_frame, trigger_entity->x, trigger_entity->y, 
            current_level, completion);
    } else if (is_level_switching) {
        float completion = ((float)_counter_level_transition) / (float)LEVEL_TRANSITION_TIME;

        // Draw interpolated background.
        _draw_interpolate_background(completion, levels[current_level].width, levels[current_level].height);

        if (trigger_entity->type != levels[current_level].player_tile) {
            _render_tile_interpolated_transform_raw(
                trigger_entity->type, levels[current_level].player_tile, _game_frame,
                levels[current_level].player_x, levels[current_level].player_y, 0, 0, completion,
                levels[current_level].width, levels[current_level].height
            );
        } else {
            _render_tile_win_animation(
                trigger_entity->type, _game_frame, trigger_entity->x, trigger_entity->y, 
                current_level, 1);
        }
    } else if (is_level_starting) {
        float completion = 1 - (((float)_counter_level_transition) / (float)LEVEL_START_TIME);

        // Fade out.
        _draw_fade_background(completion);

        // Interpolate tile position.
        _render_tile(levels[current_level].player_tile, _game_frame, 
            levels[current_level].player_x, levels[current_level].player_y);
    }
}

int _get_entity_index() {
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        if (!entities[i].active) {
            return i;
        }
    }

    // out of entities
    puts("Too many entities!");
    quit_game();

    return 0;
}

void _add_entity_to_grid(entity *t) {
    // add to head of linked list
    entity *curr = game_grid[t->y][t->x];
    t->next = curr;
    t->prev = NULL;
    if (curr) curr->prev = t;
    game_grid[t->y][t->x] = t;
}

void _remove_entity(entity *t) {
    // unlink
    if (t->prev) {
        t->prev->next = t->next;
        t->next->prev = t->prev;
    } else {
        // new head
        game_grid[t->y][t->x] = t->next;
        t->next->prev = NULL;
    }
}

void _spawn_entity(int x, int y, tile_type tile) {
    int idx = _get_entity_index();
    entities[idx].active = true;
    entities[idx].type = tile;
    entities[idx].is_moving = false;
    entities[idx].x = x;
    entities[idx].y = y;

    switch (tile) {
        case TILE_W_FLAG:
        case TILE_W_IS:
        case TILE_W_SICE:
        case TILE_W_STOP:
        case TILE_W_THONK:
        case TILE_W_WALL:
        case TILE_W_YOU:
        case TILE_W_DICE:
        case TILE_S_CIRCLE_DOT:
        case TILE_S_D:
        case TILE_S_E:
        case TILE_S_FALSE:
        case TILE_S_BACK_L:
        case TILE_S_FILLED_TRIANGLE:
        case TILE_S_J:
        case TILE_S_X:
        case TILE_S_N:
        case TILE_S_G:
        case TILE_S_TRIANGLE:
        case TILE_S_HALF_SQUARE:
        case TILE_S_Z:
        case TILE_S_PLUS:
        case TILE_S_SQUARE:
        case TILE_S_TARGET:
        case TILE_S_SPLIT_CIRCLE:
        case TILE_S_BACK_R:
        case TILE_S_T:
        case TILE_S_PI:
        case TILE_S_SLASH:
        case TILE_S_I:
        case TILE_S_K:
        case TILE_S_F:
        case TILE_S_Y:
        case TILE_W_CODE_RIGHT:
        case TILE_W_CODE_DOWN:
        case TILE_NO_CHECK:
        case TILE_CHECK:
        case TILE_W_CHECK:
            entities[idx].is_text = true;
            break;
        default:
            break;
    }

    _add_entity_to_grid(&entities[idx]);
}

void _end_move() {
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        if (entities[i].active) {
            if (entities[i].is_moving) {
                _remove_entity(&entities[i]);

                if (!is_replay) {
                    store_movement(&entities[i], entities[i].dx, entities[i].dy);
                }

                entities[i].x += entities[i].dx;
                entities[i].y += entities[i].dy;
                entities[i].is_moving = false;
                entities[i].dx = 0;
                entities[i].dy = 0;

                _add_entity_to_grid(&entities[i]);
            }

            if (entities[i].is_transforming) {
                if (!is_replay) {
                    store_transform(&entities[i], entities[i].type);
                }
                entities[i].type = entities[i].next_type;
                entities[i].is_transforming = false;
            }
        }
    }

    // step memory cell
    if (!is_replay) {
        next_move();
    }

    if (pre_dx != 0 || pre_dy != 0) {
        _try_move(pre_dx, pre_dy);
        pre_dx = 0;
        pre_dy = 0;
    }
}

void _reset_all() {
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        entities[i].active = false;
        _remove_entity(&entities[i]);

        memset(&entities[i], 0, sizeof(entity));
    }
}

void _start_level(level lev) {
    _reset_all();

    GAME_WIDTH = lev.width;
    GAME_HEIGHT = lev.height;
    lev.init_level();

    // For preview
    update_rules(entities, game_grid);

    // Set to get a rule update on the first tick.
    has_move = true;
}

void init_game() {
    level curr = levels[current_level];
    _start_level(curr);
}

void _complete(entity *trigger) {
    printf("Win!\n");
    win_level();

    switch_level(trigger, 0);
}

void update_game(unsigned long delta) {
    _counter_tile_animation += (delta / 1000);

    // check for tile animation
    if (_counter_tile_animation > TILE_ANIMATION_DELAY) {
        _game_frame = (_game_frame + 1) % NUM_FRAMES;
        _counter_tile_animation %= TILE_ANIMATION_DELAY;
    }

    bool block_movement = (
        is_level_complete || is_level_switching || is_level_starting
    );

    // check for movement
    if (has_move && !block_movement) {
        _counter_move += (delta / 1000);
        if (_counter_move > MOVE_DELAY) {
            // reset movement
            _counter_move = 0;
            has_move = false;
            _end_move();
            bool needs_move = update_rules(entities, game_grid);
            // if (needs_move && !ghost_move) {
            //     ghost_move = true;
            //     has_move = true;
            // }

            // Level-specific rules.
            level curr = levels[current_level];
            if (curr.extra_rules) {
                curr.extra_rules(entities, game_grid);
            }

            entity *trigger = 0;
            if (check_win_condition(entities, game_grid, &trigger)) {
                _complete(trigger);
            }
        }
    }

    // check for level switch
    if (is_level_complete) {
        // Fade out
        _counter_level_transition += (delta / 1000);
        if (_counter_level_transition > LEVEL_END_TIME) {
            _counter_level_transition = 0;

            is_level_complete = false;
            is_level_switching = true;
        }
    } else if (is_level_switching) {
        _counter_level_transition += (delta / 1000);
        if (_counter_level_transition > LEVEL_TRANSITION_TIME) {
            _counter_level_transition = 0;

            is_level_switching = false;
            is_level_starting = true;
            init_game();
        }
    } else if (is_level_starting) {
        _counter_level_transition += (delta / 1000);
        if (_counter_level_transition > LEVEL_START_TIME) {
            _counter_level_transition = 0;

            is_level_starting = false;
        }
    }
}

// try to move an entity in a direction
// if check is true, don't actually make the movement
bool _try_move_entity(entity *t, int dx, int dy, bool check) {
    // returns true if entity was moved
    int tx = t->x + dx;
    int ty = t->y + dy;

    // bounds check
    if (tx < 0 || ty < 0 || tx >= GAME_WIDTH || ty >= GAME_HEIGHT) return false;

    // check for obstacles
    entity *o = game_grid[ty][tx];
    bool can_move = true;
    while (o) {
        if (o->active && o->can_block && !o->can_push && !o->can_control) {
            // non-movable object
            can_move = false;
            break;
        }

        if (o->active && o->can_push) {
            // moveable object
            if (!_try_move_entity(o, dx, dy, check)) {
                // next entity was blocked
                can_move = false;
            }
        }

        o = o->next;
    }

    if (!can_move) return false;

    // apply movement
    if (!check) {
        t->is_moving = true;
        t->dx = dx;
        t->dy = dy;
    }

    return true;
}

void _try_move(int dx, int dy) {
    if (has_move) {
        // set pre-move
        pre_dx = dx;
        pre_dy = dy;
        return;
    }

    // otherwise set move
    has_move = true;
    ghost_move = false;
    is_replay = false;
    printf("Move [%d, %d]\n", dx, dy);

    // move controllable entities
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        if (entities[i].active && entities[i].can_control && !entities[i].is_moving) {
            if (_try_move_entity(&entities[i], dx, dy, true)) {
                _try_move_entity(&entities[i], dx, dy, false);
            }
        }
    }
}

void _try_replay() {
    if (!memory_available()) return;
    if (has_move) return;

    has_move = true;
    is_replay = true;
    printf("Replay\n");

    apply_previous();
}


void handle_events() {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0){
        if(e.type == SDL_QUIT){
            quit_game();
        }
        else if(e.type == SDL_KEYDOWN){
            switch(e.key.keysym.sym){
                case SDLK_d:
                case SDLK_RIGHT:
                    _try_move(1,0);
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                    _try_move(-1,0);
                    break;
                case SDLK_w:
                case SDLK_UP:
                    _try_move(0,-1);
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    _try_move(0,1);
                    break;
                case SDLK_SPACE:
                    _try_move(0,0);
                    break;
                case SDLK_z:
                    _try_replay();
                    break;
                case SDLK_r:
                    _start_level(levels[current_level]);
                    is_level_starting = true;
                    break;
                case SDLK_q:
                    current_level = 0;
                    _start_level(levels[current_level]);
                    is_level_starting = true;
                    break;
            }
        }
    }
}

void switch_level(entity *trigger, int next_level) {
    is_level_complete = true;
    _counter_level_transition = 0;

    trigger_entity = trigger;

    current_level = next_level;
}
