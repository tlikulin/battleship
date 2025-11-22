#pragma once

/*
 * Declares types and functions related to the boards.
 * I.e., initializing, displaying, taking shots at.
 */

// The size of the boards, changing it would still require to amend the
// number/size of ships, saving/loading part and (if it becomes 2 digits)
// display of column numbers.
#define GRID_SIZE 8
// The maximum number of characters allowed in the name field
#define NAME_LEN 20

// The values a tile can have, represented not as characters but IDs.
enum tile_type {
    TILE_NONE = 0,
    TILE_WATER,
    TILE_WATER_HIT,
    TILE_SHIP,
    TILE_SHIP_HIT
};

// A board storing a grid (2d array) of tiles, name of the owner and statistics
// (number of shots and hits on THIS board, not by the owner).
typedef struct board {
    unsigned char grid[GRID_SIZE][GRID_SIZE];
    char name[NAME_LEN + 1];
    int targets_left;
    int shots, hits;
} board_t;

// Represents the outcome of a shot.
// Returned by take_shot()/computer_take_shot()
enum shot_result {
    SHOT_INVALID = 0,
    SHOT_ALREADY,
    SHOT_MISSED,
    SHOT_HIT,
    SHOT_SHIP_DOWN,
    SHOT_WIN
};

// Initializes a board: clears and then populates with ships, copies name.
void init_board(board_t* board, const char* name);
// Prints 2 boards side-to-side with given visibility (determines whose
// undiscovered ships can be seen).
void print_boards(board_t* board1, board_t* board2, int visibility);
// Shoot at the board at given coords.
// Returns the outcome (descibed at the enum).
enum shot_result take_shot(board_t* board, int y, int x);
// Let computer generate random (but valid) coords and shoot at it.
// Returns the outcome (descibed at the enum), some values cannot be returned
// from this function.
enum shot_result computer_take_shot(board_t* board);