#pragma once

/* 
* Declares types and functions related to the player's board.
*/

// should work in the range [?, 9]
#define GRID_SIZE 8
// if defined, there will be no -+-+- .. +- between rows
#define NO_ROWS_SEPARATOR
// Ships: 5 4 3 2 2
#define SHIPS_TOTAL 5
extern const int SHIPS_SIZES[SHIPS_TOTAL];

// the values a tile can have
typedef enum tile_type {
    TILE_NONE = 0,
    TILE_WATER,
    TILE_WATER_HIT,
    TILE_SHIP,
    TILE_SHIP_HIT
} tile_type_t;

// a board storing a grid (2d array) of tiles
typedef struct board {
    unsigned char grid[GRID_SIZE][GRID_SIZE];
} board_t;

// initializes a board (clears and then populates with ships)
void init_board(board_t*);
// prints the repr of the board to stdout with column and row headings and separators
void print_board(board_t*);

