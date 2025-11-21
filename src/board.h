#pragma once

/*
 * Declares types and functions related to the player's board.
 */

// should work in the range [?, 9]
#define GRID_SIZE 8
// Ships: 5 4 3 2 2
#define SHIPS_TOTAL 5
extern const int SHIPS_SIZES[SHIPS_TOTAL];
// the maximum number of characters allowed in the name field
#define NAME_LEN 20

// the values a tile can have
enum tile_type {
    TILE_NONE = 0,
    TILE_WATER,
    TILE_WATER_HIT,
    TILE_SHIP,
    TILE_SHIP_HIT
};

// a board storing a grid (2d array) of tiles
typedef struct board {
    unsigned char grid[GRID_SIZE][GRID_SIZE];
    char name[NAME_LEN + 1];
    int targets_left;
    int shots, hits;
} board_t;

// represents the outcome of a shot; returned by take_shot()
enum shot_result {
    SHOT_INVALID = 0,
    SHOT_ALREADY,
    SHOT_MISSED,
    SHOT_HIT,
    SHOT_SHIP_DOWN,
    SHOT_WIN
};

// initializes a board (clears and then populates with ships)
void init_board(board_t* board, const char* name);
// prints 2 boards side-to-side with provided titles (if any)
void print_boards(board_t* board1, board_t* board2, int visibility);
// shoot at the board at given coords
// returns the outcome (descibed at the enum)
enum shot_result take_shot(board_t* board, int y, int x);
enum shot_result computer_take_shot(board_t* board);