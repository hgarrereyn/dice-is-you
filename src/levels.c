
#include "levels.h"
#include "tiles.h"
#include "game.h"
#include "rules.h"
#include "entity.h"
#include "sha-256.h"

#include <stdio.h>
#include <emscripten.h>

void _spawn_entity(int x, int y, tile_type tile);


bool did_print_flag = false;

int max_level_unlocked = 1;


void menu_level() {
    _spawn_entity(1,1,TILE_DICE);
    _spawn_entity(2,1,TILE_S_D);
    _spawn_entity(3,1,TILE_S_FALSE);
    _spawn_entity(4,1,TILE_S_SQUARE);
    _spawn_entity(5,1,TILE_S_E);
    _spawn_entity(6,1,TILE_S_G);
    _spawn_entity(7,1,TILE_S_TRIANGLE);
    _spawn_entity(8,1,TILE_S_N);
    _spawn_entity(9,1,TILE_S_G);
    _spawn_entity(10,1,TILE_DICE);

    _spawn_entity(1,3,TILE_WALL);
    _spawn_entity(2,3,TILE_WALL);
    _spawn_entity(3,3,TILE_WALL);
    _spawn_entity(4,3,TILE_WALL);
    _spawn_entity(5,3,TILE_WALL);
    _spawn_entity(6,3,TILE_WALL);
    _spawn_entity(7,3,TILE_WALL);
    _spawn_entity(8,3,TILE_WALL);
    _spawn_entity(9,3,TILE_WALL);
    _spawn_entity(10,3,TILE_WALL);

    _spawn_entity(1,4,TILE_WALL);
    _spawn_entity(1,5,TILE_WALL);
    _spawn_entity(2,5,TILE_WALL);
    _spawn_entity(3,5,TILE_WALL);
    _spawn_entity(4,5,TILE_WALL);
    _spawn_entity(5,5,TILE_WALL);
    _spawn_entity(6,5,TILE_WALL);
    _spawn_entity(6,6,TILE_WALL);
    _spawn_entity(6,7,TILE_WALL);
    _spawn_entity(6,8,TILE_WALL);
    _spawn_entity(6,9,TILE_WALL);
    _spawn_entity(6,10,TILE_WALL);
    _spawn_entity(6,11,TILE_WALL);
    _spawn_entity(6,12,TILE_WALL);
    _spawn_entity(6,13,TILE_WALL);

    _spawn_entity(7,13,TILE_WALL);
    _spawn_entity(8,13,TILE_WALL);
    _spawn_entity(9,13,TILE_WALL);
    _spawn_entity(10,13,TILE_WALL);

    _spawn_entity(10,4,TILE_WALL);
    _spawn_entity(10,5,TILE_WALL);
    _spawn_entity(10,6,TILE_WALL);
    _spawn_entity(10,7,TILE_WALL);
    _spawn_entity(10,8,TILE_WALL);
    _spawn_entity(10,9,TILE_WALL);
    _spawn_entity(10,10,TILE_WALL);
    _spawn_entity(10,11,TILE_WALL);
    _spawn_entity(10,12,TILE_WALL);

    _spawn_entity(8,5,TILE_WALL);
    _spawn_entity(9,5,TILE_WALL);
    _spawn_entity(8,7,TILE_WALL);
    _spawn_entity(9,7,TILE_WALL);
    _spawn_entity(8,9,TILE_WALL);
    _spawn_entity(9,9,TILE_WALL);
    _spawn_entity(8,11,TILE_WALL);
    _spawn_entity(9,11,TILE_WALL);

    _spawn_entity(2,7,TILE_W_THONK);
    _spawn_entity(3,7,TILE_W_IS);
    _spawn_entity(4,7,TILE_W_YOU);

    _spawn_entity(2,9,TILE_W_WALL);
    _spawn_entity(3,9,TILE_W_IS);
    _spawn_entity(4,9,TILE_W_STOP);

    _spawn_entity(9,4,TILE_W_1);
    _spawn_entity(9,6,TILE_W_2);
    _spawn_entity(9,8,TILE_W_3);
    _spawn_entity(9,10,TILE_W_4);
    _spawn_entity(9,12,TILE_W_5);

    _spawn_entity(2,4,TILE_THONK);
}

void _transition_if_player_in_chain(entity *chain, int next_level) {
    while (chain) {
        if (chain->can_control) {
            switch_level(chain, next_level);
        }
        chain = chain->next;
    }
}

void menu_level_rules(entity entities[], entity *game_grid[MAX_HEIGHT][MAX_WIDTH]) {
    if (max_level_unlocked >= 1) {
        for (int i = 0; i < MAX_ENTITIES; ++i) {
            if (entities[i].type == TILE_W_1) {
                entities[i].is_active_text = true;
            }
        }
        _transition_if_player_in_chain(game_grid[4][9], 1);
    }

    if (max_level_unlocked >= 2) {
        for (int i = 0; i < MAX_ENTITIES; ++i) {
            if (entities[i].type == TILE_W_2) {
                entities[i].is_active_text = true;
            }
        }
        _transition_if_player_in_chain(game_grid[6][9], 2);
    }    

    if (max_level_unlocked >= 3) {
        for (int i = 0; i < MAX_ENTITIES; ++i) {
            if (entities[i].type == TILE_W_3) {
                entities[i].is_active_text = true;
            }
        }
        _transition_if_player_in_chain(game_grid[8][9], 3);
    }

    if (max_level_unlocked >= 4) {
        for (int i = 0; i < MAX_ENTITIES; ++i) {
            if (entities[i].type == TILE_W_4) {
                entities[i].is_active_text = true;
            }
        }
        _transition_if_player_in_chain(game_grid[10][9], 4);
    }

    if (max_level_unlocked >= 5) {
        for (int i = 0; i < MAX_ENTITIES; ++i) {
            if (entities[i].type == TILE_W_5) {
                entities[i].is_active_text = true;
            }
        }
        _transition_if_player_in_chain(game_grid[12][9], 5);
    }
}

void level1() {
    _spawn_entity(1,1,TILE_WALL);
    _spawn_entity(2,1,TILE_WALL);
    _spawn_entity(3,1,TILE_WALL);
    _spawn_entity(4,1,TILE_WALL);
    _spawn_entity(5,1,TILE_WALL);
    _spawn_entity(6,1,TILE_WALL);
    _spawn_entity(7,1,TILE_WALL);
    _spawn_entity(1,7,TILE_WALL);
    _spawn_entity(2,7,TILE_WALL);
    _spawn_entity(3,7,TILE_WALL);
    _spawn_entity(4,7,TILE_WALL);
    _spawn_entity(5,7,TILE_WALL);
    _spawn_entity(6,7,TILE_WALL);
    _spawn_entity(7,7,TILE_WALL);
    _spawn_entity(1,2,TILE_WALL);
    _spawn_entity(1,3,TILE_WALL);
    _spawn_entity(1,4,TILE_WALL);
    _spawn_entity(1,5,TILE_WALL);
    _spawn_entity(1,6,TILE_WALL);
    _spawn_entity(7,2,TILE_WALL);
    _spawn_entity(7,3,TILE_WALL);
    _spawn_entity(7,4,TILE_WALL);
    _spawn_entity(7,5,TILE_WALL);
    _spawn_entity(7,6,TILE_WALL);

    _spawn_entity(13,1,TILE_WALL);
    _spawn_entity(13,2,TILE_WALL);
    _spawn_entity(13,3,TILE_WALL);
    _spawn_entity(13,4,TILE_WALL);
    _spawn_entity(13,5,TILE_WALL);
    _spawn_entity(13,6,TILE_WALL);
    _spawn_entity(13,7,TILE_WALL);
    _spawn_entity(15,1,TILE_WALL);
    _spawn_entity(15,2,TILE_WALL);
    _spawn_entity(15,3,TILE_WALL);
    _spawn_entity(15,4,TILE_WALL);
    _spawn_entity(15,5,TILE_WALL);
    _spawn_entity(15,6,TILE_WALL);
    _spawn_entity(15,7,TILE_WALL);
    _spawn_entity(14,1,TILE_WALL);
    _spawn_entity(14,7,TILE_WALL);

    _spawn_entity(4,2,TILE_THONK);
    _spawn_entity(3,5,TILE_W_THONK);
    _spawn_entity(4,5,TILE_W_IS);
    _spawn_entity(5,5,TILE_W_YOU);
    _spawn_entity(5,4,TILE_W_DICE);

    _spawn_entity(9,4,TILE_W_WALL);
    _spawn_entity(10,4,TILE_W_IS);
    _spawn_entity(11,4,TILE_W_STOP);
    _spawn_entity(10,3,TILE_W_FLAG);
    _spawn_entity(10,5,TILE_W_SICE);
    
    _spawn_entity(14,6,TILE_FLAG);
    _spawn_entity(14,2,TILE_DICE);
}

void level2() {
    _spawn_entity(1,1,TILE_DICE);
    _spawn_entity(2,1,TILE_DICE);
    _spawn_entity(3,1,TILE_DICE);
    _spawn_entity(4,1,TILE_DICE);
    _spawn_entity(5,1,TILE_DICE);
    _spawn_entity(6,1,TILE_DICE);
    _spawn_entity(7,1,TILE_DICE);
    _spawn_entity(1,7,TILE_DICE);
    _spawn_entity(2,7,TILE_DICE);
    _spawn_entity(3,7,TILE_DICE);
    _spawn_entity(4,7,TILE_DICE);
    _spawn_entity(5,7,TILE_DICE);
    _spawn_entity(6,7,TILE_DICE);
    _spawn_entity(7,7,TILE_DICE);
    _spawn_entity(1,2,TILE_DICE);
    _spawn_entity(1,3,TILE_DICE);
    _spawn_entity(1,4,TILE_DICE);
    _spawn_entity(1,5,TILE_DICE);
    _spawn_entity(1,6,TILE_DICE);
    _spawn_entity(7,2,TILE_DICE);
    _spawn_entity(7,3,TILE_DICE);
    _spawn_entity(7,4,TILE_DICE);
    _spawn_entity(7,5,TILE_DICE);
    _spawn_entity(7,6,TILE_DICE);

    _spawn_entity(13,1,TILE_DICE);
    _spawn_entity(13,2,TILE_DICE);
    _spawn_entity(13,3,TILE_DICE);
    _spawn_entity(13,4,TILE_DICE);
    _spawn_entity(13,5,TILE_DICE);
    _spawn_entity(13,6,TILE_DICE);
    _spawn_entity(13,7,TILE_DICE);
    _spawn_entity(15,1,TILE_DICE);
    _spawn_entity(15,2,TILE_DICE);
    _spawn_entity(15,3,TILE_DICE);
    _spawn_entity(15,4,TILE_DICE);
    _spawn_entity(15,5,TILE_DICE);
    _spawn_entity(15,6,TILE_DICE);
    _spawn_entity(15,7,TILE_DICE);
    _spawn_entity(14,1,TILE_DICE);
    _spawn_entity(14,7,TILE_DICE);

    _spawn_entity(4,2,TILE_FLAG);
    _spawn_entity(3,5,TILE_W_FLAG);
    _spawn_entity(4,5,TILE_W_IS);
    _spawn_entity(5,5,TILE_W_YOU);
    _spawn_entity(5,4,TILE_W_CHECK);

    _spawn_entity(9,4,TILE_W_DICE);
    _spawn_entity(10,4,TILE_W_IS);
    _spawn_entity(11,4,TILE_W_STOP);
    _spawn_entity(10,3,TILE_W_THONK);
    _spawn_entity(10,5,TILE_W_SICE);
    
    _spawn_entity(14,6,TILE_THONK);
    _spawn_entity(14,2,TILE_CHECK);
}

void level3() {
    _spawn_entity(1,1,TILE_WALL);
    _spawn_entity(2,1,TILE_WALL);
    _spawn_entity(3,1,TILE_WALL);
    _spawn_entity(4,1,TILE_WALL);
    _spawn_entity(5,1,TILE_WALL);
    _spawn_entity(6,1,TILE_WALL);
    _spawn_entity(7,1,TILE_WALL);
    _spawn_entity(1,7,TILE_WALL);
    _spawn_entity(2,7,TILE_WALL);
    _spawn_entity(3,7,TILE_WALL);
    _spawn_entity(4,7,TILE_WALL);
    _spawn_entity(5,7,TILE_WALL);
    _spawn_entity(6,7,TILE_WALL);
    _spawn_entity(7,7,TILE_WALL);
    _spawn_entity(1,2,TILE_WALL);
    _spawn_entity(1,3,TILE_WALL);
    _spawn_entity(1,4,TILE_WALL);
    _spawn_entity(1,5,TILE_WALL);
    _spawn_entity(1,6,TILE_WALL);
    _spawn_entity(7,2,TILE_WALL);
    _spawn_entity(7,3,TILE_WALL);
    _spawn_entity(7,4,TILE_WALL);
    _spawn_entity(7,5,TILE_WALL);
    _spawn_entity(7,6,TILE_WALL);

    _spawn_entity(13,1,TILE_WALL);
    _spawn_entity(13,2,TILE_WALL);
    _spawn_entity(13,3,TILE_WALL);
    _spawn_entity(13,4,TILE_WALL);
    _spawn_entity(13,5,TILE_WALL);
    _spawn_entity(13,6,TILE_WALL);
    _spawn_entity(13,7,TILE_WALL);
    _spawn_entity(15,1,TILE_WALL);
    _spawn_entity(15,2,TILE_WALL);
    _spawn_entity(15,3,TILE_WALL);
    _spawn_entity(15,4,TILE_WALL);
    _spawn_entity(15,5,TILE_WALL);
    _spawn_entity(15,6,TILE_WALL);
    _spawn_entity(15,7,TILE_WALL);
    _spawn_entity(14,1,TILE_WALL);
    _spawn_entity(14,7,TILE_WALL);

    _spawn_entity(4,2,TILE_THONK);
    _spawn_entity(3,5,TILE_W_THONK);
    _spawn_entity(4,5,TILE_W_IS);
    _spawn_entity(5,5,TILE_W_YOU);
    _spawn_entity(5,4,TILE_W_DICE);

    _spawn_entity(10,4,TILE_W_IS);
    _spawn_entity(10,3,TILE_W_FLAG);
    _spawn_entity(10,5,TILE_W_SICE);
    
    _spawn_entity(14,6,TILE_FLAG);
}

void level4() {
    _spawn_entity(1,5,TILE_WALL);
    _spawn_entity(2,5,TILE_WALL);
    _spawn_entity(3,5,TILE_WALL);
    _spawn_entity(4,5,TILE_WALL);
    _spawn_entity(5,5,TILE_WALL);
    _spawn_entity(6,5,TILE_WALL);
    _spawn_entity(7,5,TILE_WALL);
    _spawn_entity(8,5,TILE_WALL);
    _spawn_entity(9,5,TILE_WALL);
    
    _spawn_entity(1,6,TILE_WALL);
    _spawn_entity(1,7,TILE_WALL);
    _spawn_entity(1,8,TILE_WALL);
    _spawn_entity(1,9,TILE_WALL);
    _spawn_entity(1,10,TILE_WALL);
    _spawn_entity(1,11,TILE_WALL);
    _spawn_entity(1,12,TILE_WALL);
    _spawn_entity(1,13,TILE_WALL);

    _spawn_entity(9,6,TILE_WALL);
    _spawn_entity(9,7,TILE_WALL);
    _spawn_entity(9,8,TILE_WALL);
    _spawn_entity(9,9,TILE_WALL);
    _spawn_entity(9,10,TILE_WALL);
    _spawn_entity(9,11,TILE_WALL);
    _spawn_entity(9,12,TILE_WALL);
    _spawn_entity(9,13,TILE_WALL);

    _spawn_entity(2,13,TILE_WALL);
    _spawn_entity(3,13,TILE_WALL);
    _spawn_entity(4,13,TILE_WALL);
    _spawn_entity(5,13,TILE_WALL);
    _spawn_entity(6,13,TILE_WALL);
    _spawn_entity(7,13,TILE_WALL);
    _spawn_entity(8,13,TILE_WALL);

    _spawn_entity(4,8,TILE_WALL);
    _spawn_entity(5,8,TILE_WALL);
    _spawn_entity(6,8,TILE_WALL);
    _spawn_entity(7,8,TILE_WALL);
    _spawn_entity(8,8,TILE_WALL);

    _spawn_entity(2,10,TILE_WALL);
    _spawn_entity(3,10,TILE_WALL);
    _spawn_entity(4,10,TILE_WALL);
    _spawn_entity(5,10,TILE_WALL);
    _spawn_entity(6,10,TILE_WALL);

    _spawn_entity(2,1,TILE_W_THONK);
    _spawn_entity(3,1,TILE_W_IS);
    _spawn_entity(4,1,TILE_W_DICE);
    _spawn_entity(5,1,TILE_W_IS);
    _spawn_entity(6,1,TILE_W_FLAG);
    _spawn_entity(7,1,TILE_W_IS);
    _spawn_entity(8,1,TILE_W_THONK);
    _spawn_entity(4,2,TILE_W_IS);
    _spawn_entity(4,3,TILE_W_YOU);

    _spawn_entity(6,3,TILE_W_WALL);
    _spawn_entity(7,3,TILE_W_IS);
    _spawn_entity(8,3,TILE_W_STOP);

    _spawn_entity(8,6,TILE_THONK);
    _spawn_entity(7,7,TILE_W_FLAG);
    _spawn_entity(7,10,TILE_W_IS);
    _spawn_entity(3,11,TILE_W_SICE);
    _spawn_entity(2,12,TILE_DICE);
}

void level_flag_fin() {
    _spawn_entity(2,2,TILE_NO_CHECK);

    _spawn_entity(1,1,TILE_WALL);
    _spawn_entity(2,1,TILE_WALL);
    _spawn_entity(3,1,TILE_WALL);
    _spawn_entity(4,1,TILE_WALL);
    _spawn_entity(5,1,TILE_WALL);
    _spawn_entity(6,1,TILE_WALL);
    _spawn_entity(7,1,TILE_WALL);
    _spawn_entity(8,1,TILE_WALL);
    _spawn_entity(8,2,TILE_WALL);
    _spawn_entity(1,2,TILE_WALL);
    _spawn_entity(1,3,TILE_WALL);
    _spawn_entity(1,4,TILE_WALL);
    _spawn_entity(1,5,TILE_WALL);
    _spawn_entity(1,6,TILE_WALL);
    _spawn_entity(1,7,TILE_WALL);
    _spawn_entity(1,8,TILE_WALL);
    _spawn_entity(2,8,TILE_WALL);

    _spawn_entity(3,3,TILE_S_Y);
    _spawn_entity(4,3,TILE_S_X);
    _spawn_entity(5,3,TILE_S_TARGET);
    _spawn_entity(3,4,TILE_S_E);
    _spawn_entity(3,5,TILE_S_TRIANGLE);
    

    // Uncomment to have a close-solution for testing

    /*
    _spawn_entity(6,3,TILE_S_PLUS);
    _spawn_entity(7,3,TILE_S_PI);

    _spawn_entity(4,4,TILE_S_Z);
    _spawn_entity(5,4,TILE_S_J);
    _spawn_entity(6,4,TILE_S_BACK_L);
    _spawn_entity(7,4,TILE_S_K);

    _spawn_entity(4,5,TILE_S_SQUARE);
    _spawn_entity(5,5,TILE_S_HALF_SQUARE);
    _spawn_entity(6,5,TILE_S_T);
    _spawn_entity(7,5,TILE_S_FALSE);

    _spawn_entity(3,6,TILE_S_G);
    _spawn_entity(4,6,TILE_S_SPLIT_CIRCLE);
    _spawn_entity(5,6,TILE_S_D);
    _spawn_entity(6,6,TILE_S_FILLED_TRIANGLE);
    _spawn_entity(7,6,TILE_S_I);

    _spawn_entity(3,7,TILE_S_N);
    _spawn_entity(4,7,TILE_S_F);
    _spawn_entity(5,7,TILE_S_BACK_R);
    _spawn_entity(6,7,TILE_S_SLASH);
    _spawn_entity(7,8,TILE_S_CIRCLE_DOT);
    */
    

    for (int i = 1; i <= 12; ++i) {
        _spawn_entity(i,13,TILE_WALL);
        _spawn_entity(i,17,TILE_WALL);
        if (i <= 8) {
            _spawn_entity(i,15,TILE_WALL);
        }

        if (i <= 5) {
            _spawn_entity(i,19,TILE_WALL);
        }
    }

    _spawn_entity(1,14,TILE_WALL);
    _spawn_entity(1,16,TILE_WALL);
    _spawn_entity(1,18,TILE_WALL);
    _spawn_entity(8,14,TILE_WALL);
    _spawn_entity(8,16,TILE_WALL);
    _spawn_entity(5,18,TILE_WALL);
    _spawn_entity(12,14,TILE_WALL);
    _spawn_entity(12,15,TILE_WALL);
    _spawn_entity(12,16,TILE_WALL);

    _spawn_entity(2,14,TILE_W_CODE_RIGHT);
    _spawn_entity(3,14,TILE_S_SPLIT_CIRCLE);
    _spawn_entity(4,14,TILE_S_PLUS);
    _spawn_entity(5,14,TILE_S_X);
    _spawn_entity(6,14,TILE_S_Y);
    _spawn_entity(7,14,TILE_S_TARGET);

    _spawn_entity(2,16,TILE_W_CODE_RIGHT);
    _spawn_entity(3,16,TILE_S_FILLED_TRIANGLE);
    _spawn_entity(4,16,TILE_S_SQUARE);
    _spawn_entity(5,16,TILE_S_G);
    _spawn_entity(6,16,TILE_S_T);
    _spawn_entity(7,16,TILE_S_FALSE);

    _spawn_entity(10,14,TILE_W_DICE);
    _spawn_entity(10,15,TILE_W_IS);
    _spawn_entity(10,16,TILE_W_YOU);
    _spawn_entity(9,15,TILE_W_WALL);
    _spawn_entity(11,15,TILE_W_STOP);

    _spawn_entity(2,18,TILE_W_CHECK);
    _spawn_entity(3,18,TILE_W_IS);
    _spawn_entity(4,18,TILE_W_SICE);

    _spawn_entity(16,3,TILE_S_CIRCLE_DOT);
    _spawn_entity(18,3,TILE_S_D);
    _spawn_entity(10,3,TILE_S_I);
    _spawn_entity(12,3,TILE_S_FALSE);
    _spawn_entity(14,3,TILE_S_BACK_L);

    _spawn_entity(16,5,TILE_S_FILLED_TRIANGLE);
    _spawn_entity(18,5,TILE_S_J);
    _spawn_entity(10,5,TILE_S_K);
    _spawn_entity(12,5,TILE_S_F);
    _spawn_entity(14,5,TILE_S_G);

    _spawn_entity(16,7,TILE_S_N);
    _spawn_entity(18,7,TILE_S_HALF_SQUARE);
    _spawn_entity(10,7,TILE_S_Z);
    _spawn_entity(12,7,TILE_S_PLUS);
    _spawn_entity(14,7,TILE_S_SQUARE);

    _spawn_entity(16,9,TILE_S_SLASH);
    _spawn_entity(18,9,TILE_S_SPLIT_CIRCLE);
    _spawn_entity(10,9,TILE_S_BACK_R);
    _spawn_entity(12,9,TILE_S_T);
    _spawn_entity(14,9,TILE_S_PI);

    _spawn_entity(2,3,TILE_W_CODE_RIGHT);
    _spawn_entity(2,4,TILE_W_CODE_RIGHT);
    _spawn_entity(2,5,TILE_W_CODE_RIGHT);
    _spawn_entity(2,6,TILE_W_CODE_RIGHT);
    _spawn_entity(2,7,TILE_W_CODE_RIGHT);

    _spawn_entity(3,2,TILE_W_CODE_DOWN);
    _spawn_entity(4,2,TILE_W_CODE_DOWN);
    _spawn_entity(5,2,TILE_W_CODE_DOWN);
    _spawn_entity(6,2,TILE_W_CODE_DOWN);
    _spawn_entity(7,2,TILE_W_CODE_DOWN);

    _spawn_entity(2,10,TILE_DICE);
}

void _flag_rules(entity entities[], entity *game_grid[MAX_HEIGHT][MAX_WIDTH]) {
    // Check for flag completion.
    int num = 0;
    if (game_grid[3][2]->is_active_text) num += 1;
    if (game_grid[4][2]->is_active_text) num += 1;
    if (game_grid[5][2]->is_active_text) num += 1;
    if (game_grid[6][2]->is_active_text) num += 1;
    if (game_grid[7][2]->is_active_text) num += 1;

    if (game_grid[2][3]->is_active_text) num += 1;
    if (game_grid[2][4]->is_active_text) num += 1;
    if (game_grid[2][5]->is_active_text) num += 1;
    if (game_grid[2][6]->is_active_text) num += 1;
    if (game_grid[2][7]->is_active_text) num += 1;

    if (num != 10) {
        return;
    }

    game_grid[2][2]->is_active_text = true;

    if (!did_print_flag) {
        did_print_flag = true;

        unsigned char solution[25];

        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                solution[(y*5)+x] = _get_code_value(game_grid[y+3][x+3]);
            }
        }

        unsigned char hash[32];
        calc_sha_256(hash, solution, 25);

        unsigned char key[32] = {
            82, 42, 42, 16, 204, 101, 90, 18, 215, 215, 154, 106, 
            81, 85, 221, 121, 240, 235, 131, 78, 86, 182, 76, 105, 
            165, 30, 75, 16, 127, 123, 14, 4};

        unsigned char flag[33];

        for (int i = 0; i < 32; ++i) {
            flag[i] = hash[i] ^ key[i];
        }
        flag[32] = 0;
        
        char js_code[100];
        snprintf(js_code, 100, "alert('%s')", flag);

        emscripten_run_script(js_code);
        printf("flag: %s\n", flag);
    }
}

level levels[] = {
    {
        .level_idx=0,
        .width=12,
        .height=15,
        .player_tile=TILE_THONK,
        .player_x=2,
        .player_y=4,
        .init_level=menu_level,
        .extra_rules=menu_level_rules
    },
    {
        .level_idx=1,
        .width=17,
        .height=9,
        .player_tile=TILE_THONK,
        .player_x=4,
        .player_y=2,
        .init_level=level1,
        .extra_rules=0
    },
    {
        .level_idx=2,
        .width=17,
        .height=9,
        .player_tile=TILE_FLAG,
        .player_x=4,
        .player_y=2,
        .init_level=level2,
        .extra_rules=0
    },
    {
        .level_idx=3,
        .width=17,
        .height=9,
        .player_tile=TILE_THONK,
        .player_x=4,
        .player_y=2,
        .init_level=level3,
        .extra_rules=0
    },
    {
        .level_idx=4,
        .width=11,
        .height=15,
        .player_tile=TILE_THONK,
        .player_x=8,
        .player_y=6,
        .init_level=level4,
        .extra_rules=0
    },
    {
        .level_idx=5,
        .width=20,
        .height=21,
        .player_tile=TILE_DICE,
        .player_x=2,
        .player_y=10,
        .init_level=level_flag_fin,
        .extra_rules=_flag_rules
    }
};

int current_level = 0;


void load_cookie() {
    int max_unlocked = EM_ASM_INT({
        var part = document.cookie.split('unlocked=')[1];
        if (typeof(part) == 'undefined') {
            return 0;
        } else {
            return parseInt(part.split(';')[0]);
        }
    });

    max_level_unlocked = 1;
    if (max_unlocked > max_level_unlocked) {
        max_level_unlocked = max_unlocked;
    }
}

void save_cookie() {
    EM_ASM({
        document.cookie = 'unlocked=' + $0 + ';';
    }, max_level_unlocked);
}

void win_level() {
    int new_max = current_level + 1;
    max_level_unlocked = new_max > max_level_unlocked ? new_max : max_level_unlocked;

    save_cookie();
}
