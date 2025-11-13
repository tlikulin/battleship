#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"

// LOCAL HEPLERS, not public API

const int SHIPS_SIZES[SHIPS_TOTAL] = {4, 3, 3, 2, 2, 1, 1};

// get the character representation of a given tile type
char tile_to_char(tile_type_t);
// create a new board and initializes it to some empty state
void clear_board(board_t*);
// fills the board with the ships randomly
void populate_board(board_t*);
// checks if coords are inbounds
int is_inbounds(int y, int x);
// checks if a cell is touching (incl. diagonally and itself) a ship tile
int is_touching_ship(board_t*, int, int);
// checks if a ship with given type, orientation and coords can be placed
int can_place_ship(board_t*, int, int, int, int);
// places a ship with given type, orientation and coords 
// overrides old tiles, can_place_ship is expected to be run before
void place_ship(board_t*, int, int, int, int);

char tile_to_char(tile_type_t tile) {
    switch (tile) {
    case TILE_WATER:
        return ' ';
    case TILE_WATER_HIT:
        return 'O';
    case TILE_SHIP:
        return 's';
    case TILE_SHIP_HIT:
        return 'X';
    case TILE_NONE:
    default:
        return '?';
    }
}

int is_inbounds(int y, int x) {
    return 0 <= x && x < GRID_SIZE && 0 <= y && y < GRID_SIZE;
}

int is_touching_ship(board_t* board, int y, int x) {
    for (int dy = -1; dy <= 1; ++dy)
        for (int dx = -1; dx <= 1; ++dx)
            if (is_inbounds(y + dy, x + dx) && board->grid[y + dy][x + dx] == TILE_SHIP)
                return 1;

    return 0;
}

int can_place_ship(board_t* board, int ship_size, int is_horizontal, int y, int x) {
    if (is_horizontal) {
        for (int dx = 0; dx < ship_size; ++dx) {
            if (!is_inbounds(y, x + dx) || is_touching_ship(board, y, x + dx)) {
                return 0;
            }
        }
    } else {
        for (int dy = 0; dy < ship_size; ++dy) {
            if (!is_inbounds(y + dy, x) || is_touching_ship(board, y + dy, x)) {
                return 0;
            }
        }
    }

    return 1;
}

void place_ship(board_t* board, int ship_size, int is_horizontal, int y, int x) {
    if (is_horizontal) {
        for (int dx = 0; dx < ship_size; ++dx) {
            board->grid[y][x + dx] = TILE_SHIP;
        }
    } else {
        for (int dy = 0; dy < ship_size; ++dy) {
            board->grid[y + dy][x] = TILE_SHIP;
        }
    }
}

void clear_board(board_t* board) {
    for (int y = 0; y < GRID_SIZE; ++y)
        for (int x = 0; x < GRID_SIZE; ++x)
            board->grid[y][x] = TILE_WATER;
}

void populate_board(board_t* board) {
    int ship_size, is_horizontal;
    int y, x;
    srand(time(NULL));

    for (int i = 0; i < SHIPS_TOTAL; ++i) {
        ship_size = SHIPS_SIZES[i];
        do { // generate random coords and orientation until fits
            is_horizontal = rand() % 2;
            y = rand() % GRID_SIZE;
            x = rand() % GRID_SIZE;
        } while (!can_place_ship(board, ship_size, is_horizontal, y, x));

        place_ship(board, ship_size, is_horizontal, y, x);
    }
}

void init_board(board_t* board) {
    clear_board(board);
    populate_board(board);
}

void print_board(board_t* board) {
    // print column numbers
    printf("  ");
    for (int i = 1; i <= GRID_SIZE; ++i) {
        printf("%d ", i);
    }
    printf("\n");
    // print the grid with separators
    for (int y = 0; y < GRID_SIZE; ++y) {
        printf("%c ", 'a' + y);
        // '|'-separated tiles
        for (int x = 0; x < GRID_SIZE - 1; ++x) {
            char tile = tile_to_char(board->grid[y][x]);
            printf("%c|", tile);
        }
        // last column, has no separator after
        char tile = tile_to_char(board->grid[y][GRID_SIZE  - 1]);
        printf("%c\n", tile);
        // rows separator: "-+-+ ... -", not needed after last row
#ifndef NO_ROWS_SEPARATOR
        if (y == GRID_SIZE - 1) break;
        for (int i = 0; i < GRID_SIZE - 1; ++i) {
            printf("-+");
        }
        printf("-\n");
#endif
    }
}