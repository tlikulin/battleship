#pragma once
#include "board.h"

/*
 * Contains functions for creating saves
 * and reading them.
 */

// Save format:
// name1,name2,id,targets_left1,hits1,shots1,targets_left2,hits2,shots2,grid1,grid2\n
//
// Where:
//
// name1/2: char[] max 20 (alphanum + -_)
// id: size_t
// targets_left1/2,hits1/2,shots1/2: all ints
// grid1/grid2 sequence of GRID_SIZE * GRID_SIZE digits representing enum
//  tile_type values
//
// Each save takes 1 line in the file.

// the maximum number of characters allowed in the name field of a save
#define NAME_LEN 20

// Writes a line representing a game state to the savefile.
// Returns 1 if successful, 0 if failed.
int save_game(board_t* board1, board_t* board2, const char* name1,
              const char* name2);
// Loads the saved state of boards from the record with given ID.
// Returns 1 if successful, 0 if failed.
int load_boards(int id, board_t* board1, board_t* board2, char* name1,
                char* name2);