#include <stdio.h>

// works if <= 9, double digit column numbers not accounted for
#define GRID_SIZE 8
#define NO_ROWS_SEPARATOR

typedef enum tile_type {
    NONE = 0,
    WATER,
    WATER_HIT,
    SHIP,
    SHIP_HIT
} tile_type_t;

typedef struct board {
    tile_type_t grid[GRID_SIZE][GRID_SIZE];
} board_t;

board_t create_empty_board(void) {
    board_t board;

    for (int y = 0; y < GRID_SIZE; ++y)
        for (int x = 0; x < GRID_SIZE; ++x)
            board.grid[y][x] = SHIP;
    
    return board;
}

char tile_to_char(tile_type_t tile) {
    switch (tile) {
    default:
    case NONE:
        return '?';
    case WATER:
        return ' ';
    case WATER_HIT:
        return 'O';
    case SHIP:
        return '.';
    case SHIP_HIT:
        return 'X';
    }
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

int main(void) {
    board_t player_board = create_empty_board();
    printf("Player's board:\n\n");
    print_board(&player_board);
}