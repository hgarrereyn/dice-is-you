
#include "entity.h"

#define NUM_LEVELS 6

typedef struct _level {
    int level_idx;
    int unlocked;
    int width;
    int height;
    int player_tile;
    int player_x;
    int player_y;
    void (*init_level)();
    void (*extra_rules)(entity entities[], entity *game_grid[30][30]);
} level;

extern int current_level;
extern level levels[NUM_LEVELS];

void win_level();

void load_cookie();
void save_cookie();
