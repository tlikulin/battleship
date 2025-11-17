#pragma once
#include "board.h"

/*
 * Contains functions for creating saves
 * and reading them.
 */

// Format:
// id,name,targets_left1,hits1,shots1,targets_left2,hits2,shots2,grid1,grid2\n
// Where
// id: size_t
// name: char[] max 20 (no whitespaces)
// targets_left1/2,hits1/2,shots1/2: all ints
// grid1/grid2 sequence of GRID_SIZE * GRID_SIZE digits representing enum
//  tile_type values
// Each save takes 1 line in the file.

int save_game(board_t* board1, board_t* board2, int id, const char* name);