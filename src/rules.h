
#include "tiles.h"
#include "game.h"
#include "entity.h"


#define MAX_RULE 6
#define MAX_ARGS 4

typedef struct {
    bool is_arg;
    tile_type type;
} pattern_element;

typedef struct _pattern {
    int num_args;
    pattern_element tiles[MAX_RULE];
    int length;
    bool (*fn)(entity entities[], entity *game_grid[GAME_HEIGHT][GAME_WIDTH], entity *args[MAX_ARGS], bool *needs_move);
} pattern;

bool check_win_condition(entity entities[MAX_ENTITIES], entity *game_grid[MAX_HEIGHT][MAX_WIDTH], entity **trigger);
bool update_rules(entity entities[MAX_ENTITIES], entity *game_grid[GAME_HEIGHT][GAME_WIDTH]);
unsigned char _get_code_value(entity *t);
