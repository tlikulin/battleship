#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"

// LOCAL:

// Ships: 4 3 3 2 2
#define SHIPS_TOTAL 5
const int SHIPS_SIZES[SHIPS_TOTAL] = {4, 3, 3, 2, 2};

// Represents directions in which some functions may propagate.
enum direction {
    DIRECTION_NONE = 0,
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
};

// All possible direction to iterate through.
#define DIRECTIONS_TOTAL 4
const int DIRECTIONS[DIRECTIONS_TOTAL] = {DIRECTION_UP, DIRECTION_DOWN,
                                          DIRECTION_LEFT, DIRECTION_RIGHT};

// What separates left and right boards.
const char* BOARDS_BORDERLINE = "      #      ";

// The character representation of a given tile type with given visibility.
void print_tile(enum tile_type tile, int is_own);
// Checks if the given coords are within the board.
int is_inbounds(int y, int x);
// Fills the board with the ships randomly.
void populate_board(board_t* board);
// Checks if a cell is touching (incl. diagonally and itself) a ship tile.
int is_touching_ship(board_t* board, int y, int x);
// Checks if a ship with given size, orientation and coords can be placed
int can_place_ship(board_t* board, int ship_size, int is_horizontal, int y,
                   int x);
// Places a ship with given size, orientation and coords.
// Overrides old tiles, can_place_ship() is expected to be run before.
void place_ship(board_t* board, int ship_size, int is_horizontal, int y, int x);
// Checks if the ship containting the given point has been fully destroyed.
int is_ship_downis_ship_down(board_t* board, int y, int x);
// Recursive helper for is_ship_down(), should not be called independently.
int is_ship_down_inner(board_t* board, int y, int x, enum direction direction);
// Puts discovered water tiles aroung the ship containing the given coord
// supposed to run after checking if there is indeed a sunk ship there (func
// above).
void discover_around_sunk_ship(board_t* board, int y, int x);
// Recursive helper for is_ship_down(), should not be called independently.
void discover_around_sunk_ship_inner(board_t* board, int y, int x,
                                     enum direction direction);
// Changes the tile from WATER to WATER_HIT, if can't - does nothing.
void discover_water(board_t* board, int y, int x);
// Modify *y_ptr, *x_ptr as in a step in the given direction.
void move_in_direction(int* y_ptr, int* x_ptr, enum direction direction);
// Prints space-separated column numbers above the boards.
// e.g. "  1 2 3 4 5 6 7 8"
// Note: only 1-digit board size is supported.
void print_column_numbers(void);
// Prints a row of the board with given index, includes letter at the beggining.
// e.g. "C  | | | |s|O| | "
void print_board_row(board_t* board, int y, int is_own);

// Uses ANSI escape codes to color different tile types distinctively.
// is_own = should undiscovered ships be displayed
void print_tile(enum tile_type tile, int is_own) {
    switch (tile) {
    case TILE_WATER:
        printf("%c", ' ');
        break;
    case TILE_WATER_HIT:
        // blue
        printf("\x1B[38;5;12m%c\x1B[0m", 'O');
        break;
    case TILE_SHIP:
        if (is_own) {
            // orange
            printf("\x1B[38;5;9m%c\x1B[0m", 's');
        } else {
            printf("%c", ' ');
        }
        break;
    case TILE_SHIP_HIT:
        // red
        printf("\x1B[38;5;196m%c\x1B[0m", 'X');
        break;
    default:
        // purple
        printf("\x1B[38;5;5m%c\x1B[0m", '?');
        break;
    }
}

// Does not require a specific board: size is same for all.
int is_inbounds(int y, int x) {
    return 0 <= x && x < GRID_SIZE && 0 <= y && y < GRID_SIZE;
}

// The entire 3x3 area centered at the (x, y) is checked.
int is_touching_ship(board_t* board, int y, int x) {
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (is_inbounds(y + dy, x + dx) &&
                board->grid[y + dy][x + dx] == TILE_SHIP) {
                return 1;
            }
        }
    }

    return 0;
}

// Simulates each tile of the potential ship, runs bounds and touching checks
// for it.
int can_place_ship(board_t* board, int ship_size, int is_horizontal, int y,
                   int x) {
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

void place_ship(board_t* board, int ship_size, int is_horizontal, int y,
                int x) {
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

// Should by used on empty board.
void populate_board(board_t* board) {
    int ship_size, is_horizontal;
    int y, x;

    for (int i = 0; i < SHIPS_TOTAL; ++i) {
        ship_size = SHIPS_SIZES[i];

        do { // generate random coords and orientation until fits
            if ((is_horizontal = rand() % 2)) {
                y = rand() % GRID_SIZE;
                x = rand() % (GRID_SIZE - ship_size + 1);
            } else {
                y = rand() % (GRID_SIZE - ship_size + 1);
                x = rand() % GRID_SIZE;
            }
        } while (!can_place_ship(board, ship_size, is_horizontal, y, x));

        place_ship(board, ship_size, is_horizontal, y, x);
        board->targets_left += ship_size;
    }
}

// Calls recursive checks in each direction from given tile.
int is_ship_down(board_t* board, int y, int x) {
    if (!is_inbounds(y, x) || board->grid[y][x] != TILE_SHIP_HIT) {
        return 0;
    }

    for (int i = 0; i < DIRECTIONS_TOTAL; ++i) {
        if (!is_ship_down_inner(board, y, x, DIRECTIONS[i])) {
            return 0;
        }
    }

    return 1;
}

// Recursively propagate in given direction.
// Continues until in-bounds and encounters hit ship tiles only.
int is_ship_down_inner(board_t* board, int y, int x, enum direction direction) {
    if (!is_inbounds(y, x)) {
        return 1;
    }

    switch (board->grid[y][x]) {
    case TILE_WATER:
    case TILE_WATER_HIT:
        return 1;
    case TILE_SHIP_HIT:
        move_in_direction(&y, &x, direction);
        return is_ship_down_inner(board, y, x, direction);
    case TILE_SHIP:
    default:
        return 0;
    }
}

// Calls recursive reveals in each direction from given tile.
void discover_around_sunk_ship(board_t* board, int y, int x) {
    if (!is_inbounds(y, x) || board->grid[y][x] != TILE_SHIP_HIT) {
        return;
    }

    for (int i = 0; i < DIRECTIONS_TOTAL; ++i) {
        discover_around_sunk_ship_inner(board, y, x, DIRECTIONS[i]);
    }
}

// Recursive water discovery around sunk ship.
// Discovers itself and 2 tiles perpendicular to motion.
// Continues until next tile is also ship tile.
void discover_around_sunk_ship_inner(board_t* board, int y, int x,
                                     enum direction direction) {
    if (!is_inbounds(y, x)) {
        return;
    }

    discover_water(board, y, x);

    switch (direction) {
    case DIRECTION_DOWN:
    case DIRECTION_UP:
        discover_water(board, y, x - 1);
        discover_water(board, y, x + 1);
        break;
    case DIRECTION_LEFT:
    case DIRECTION_RIGHT:
        discover_water(board, y - 1, x);
        discover_water(board, y + 1, x);
        break;
    default:
        break;
    }

    if (board->grid[y][x] == TILE_SHIP_HIT) {
        move_in_direction(&y, &x, direction);
        discover_around_sunk_ship_inner(board, y, x, direction);
    }
}

// Change water to discovered water if possible.
void discover_water(board_t* board, int y, int x) {
    if (is_inbounds(y, x) && board->grid[y][x] == TILE_WATER) {
        board->grid[y][x] = TILE_WATER_HIT;
    }
}

// advance the coords in right direction.
void move_in_direction(int* y_ptr, int* x_ptr, enum direction direction) {
    switch (direction) {
    case DIRECTION_UP:
        *y_ptr -= 1;
        break;
    case DIRECTION_DOWN:
        *y_ptr += 1;
        break;
    case DIRECTION_LEFT:
        *x_ptr -= 1;
        break;
    case DIRECTION_RIGHT:
        *x_ptr += 1;
        break;
    default:
        return;
    }
}

// GRID_SIZE must be 1-digit for the numbers to align
void print_column_numbers(void) {
    printf("  ");
    for (int i = 1; i < GRID_SIZE; ++i) {
        printf("%d ", i);
    }
    printf("%d", GRID_SIZE); // no trailing space
}

// is_own = should undiscovered ships be displayed
void print_board_row(board_t* board, int y, int is_own) {
    printf("%c ", 'A' + y); // 'A' + 0 is 'A', 'A' + 1 is 'B', etc.
    // '|'-separated tiles
    for (int x = 0; x < GRID_SIZE - 1; ++x) {
        print_tile(board->grid[y][x], is_own);
        putchar('|');
    }
    // last column, has no separator after
    print_tile(board->grid[y][GRID_SIZE - 1], is_own);
}

// EXPOSED in board.h:

// 1) fills with water;
// 2) sets stats to 0;
// 3) copies name;
// 4) generates random ships.
void init_board(board_t* board, const char* name) {
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            board->grid[y][x] = TILE_WATER;
        }
    }

    board->targets_left = 0;
    board->hits = 0;
    board->shots = 0;
    strcpy(board->name, name);

    populate_board(board);
}

// visibility: if 0th bit (& 1) is set, ships in board1 are visible,
// and if 1st bit (& 2) is set, ships in board2 are visible.
// Uses ANSI escape code to move cursor to right position.
void print_boards(board_t* board1, board_t* board2, int visibility) {
    // print titles above the boards
    printf("   %s's side\x1B[%dG%s's side\n", board1->name, 2 * GRID_SIZE + 18,
           board2->name);
    // print stats - number of shots and hits
    printf("   Progress: %d/%d\x1B[%dGProgress: %d/%d\n\n", board1->hits,
           board1->shots, 2 * GRID_SIZE + 18, board2->hits, board2->shots);
    // print column numbers
    print_column_numbers();
    printf("%s", BOARDS_BORDERLINE);
    print_column_numbers();
    putchar('\n');

    // print the grids with separators
    for (int y = 0; y < GRID_SIZE; ++y) {
        print_board_row(board1, y, visibility & 1);
        printf("%s", BOARDS_BORDERLINE);
        print_board_row(board2, y, visibility & 2);
        putchar('\n');
    }
}

// Checks the tile at given coords.
// Depending on the type, updates the board and returns the result of the shot.
enum shot_result take_shot(board_t* board, int y, int x) {
    if (!is_inbounds(y, x)) { // out-of-bounds
        return SHOT_INVALID;
    }

    switch (board->grid[y][x]) {
    case TILE_WATER_HIT:
    case TILE_SHIP_HIT:
        return SHOT_ALREADY;
    case TILE_WATER:
        board->grid[y][x] = TILE_WATER_HIT;
        board->shots += 1;
        return SHOT_MISSED;
    case TILE_SHIP:
        board->grid[y][x] = TILE_SHIP_HIT;
        board->targets_left -= 1;
        board->hits += 1;
        board->shots += 1;
        if (is_ship_down(board, y, x)) {
            discover_around_sunk_ship(board, y, x);
            // checked inside the is_ship_down if so that
            // discover_around_sunk_ship will be called before returning
            if (board->targets_left == 0) {
                return SHOT_WIN;
            } else {
                return SHOT_SHIP_DOWN;
            }
        } else {
            return SHOT_HIT;
        }
    default:
        return SHOT_INVALID; // invalid tile type
    }
}

// Random coords generation until can shoot there.
enum shot_result computer_take_shot(board_t* board) {
    int x, y;
    // keep generating random coords until can shoot
    do {
        x = rand() % GRID_SIZE;
        y = rand() % GRID_SIZE;
    } while (board->grid[y][x] != TILE_SHIP && board->grid[y][x] != TILE_WATER);

    return take_shot(board, y, x);
}