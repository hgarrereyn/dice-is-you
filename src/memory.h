
#include "entity.h"
#include "tiles.h"


typedef struct _mem_move {
    struct _mem_move *next;
    entity *t;
    int dx;
    int dy;
} mem_move;

typedef struct _mem_transform {
    struct _mem_transform *next;
    entity *t;
    tile_type old_type;
} mem_transform;

typedef struct {
    mem_move *movement;
    mem_transform *transforms;
} memory_cell;


void next_move();
bool memory_available();

/* Add movement record to current memory cell */
void store_movement(entity *t, int dx, int dy);

/* Add movement record to current memory cell */
void store_transform(entity *t, tile_type old_type);

void apply_previous();
