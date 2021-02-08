
#include "entity.h"

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

#define MAX_WIDTH 30
#define MAX_HEIGHT 30

#define GAME_BORDER 20 // pixels

#define NUM_FRAMES 3

#define MAX_ENTITIES 512


void init_game();
void render_game(unsigned long delta);
void update_game(unsigned long delta);

void switch_level(entity *trigger, int next_level);
