
#include "memory.h"

#define MEM_SIZE 256


memory_cell memory[256];
int _mem_idx = 0;
int _mem_used = 0;

void next_move() {
    // switch to the next memory cell
    _mem_idx = (_mem_idx + 1) % MEM_SIZE;
    if (_mem_used < MEM_SIZE) _mem_used += 1;
}

bool memory_available() {
    return _mem_used > 0;
}

/* Add movement record to current memory cell */
void store_movement(entity *t, int dx, int dy) {
    mem_move *record = (mem_move *)(malloc(sizeof(mem_move)));

    record->t = t;
    record->dx = dx;
    record->dy = dy;
    record->next = memory[_mem_idx].movement;

    memory[_mem_idx].movement = record;
}

/* Add movement record to current memory cell */
void store_transform(entity *t, tile_type old_type) {
    mem_transform *record = (mem_transform *)(malloc(sizeof(mem_transform)));

    record->t = t;
    record->old_type = old_type;
    record->next = memory[_mem_idx].transforms;

    memory[_mem_idx].transforms = record;
}

void _free_cell(memory_cell *cell) {
    mem_move *movement = cell->movement;
    while (movement) {
        mem_move *temp = movement->next;
        free(movement);
        movement = temp;
    }

    mem_transform *transform = cell->transforms;
    while (transform) {
        mem_transform *temp = transform->next;
        free(transform);
        transform = temp;
    }

    cell->movement = NULL;
    cell->transforms = NULL;
}

void apply_previous() {
    _mem_idx = (_mem_idx - 1) % MEM_SIZE;
    _mem_used -= 1;
    memory_cell *prev = &memory[_mem_idx];

    mem_move *movement = prev->movement;
    while (movement) {
        entity *t = movement->t;
        t->is_moving = true;
        t->dx = -movement->dx;
        t->dy = -movement->dy;

        movement = movement->next;
    }

    mem_transform *transform = prev->transforms;
    while (transform) {
        entity *t = transform->t;
        t->next_type = transform->old_type;

        transform = transform->next;
    }

    _free_cell(prev);
}
