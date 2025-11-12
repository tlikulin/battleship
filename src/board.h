#pragma once

/* 
* Declares types and functions related to the player's board.
*/

// should work in the range [?, 9]
#define GRID_SIZE 8
// if defined, there will be no -+-+- .. +- between rows
#define NO_ROWS_SEPARATOR

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

// create a new board and initializes it to some empty state
board_t create_empty_board(void);
// prints the repr of the board to stdout w/ column&row headings and separators
void print_board(board_t* board);
// get the character representation for a given tile type
char tile_to_char(tile_type_t tile);
