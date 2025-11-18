#include "save-load.h"
#include "board.h"
#include <stdio.h>
#include <stdlib.h>

const char* SAVEFILE_NAME = "battleship-savefile.txt";

// EXPOSED in saves.h:

int save_game(board_t* board1, board_t* board2, const char* name) {
    char line[512] = {0};
    int pos = RAND_MAX, id;
    FILE* savefile = fopen(SAVEFILE_NAME, "a");
    if (savefile == NULL) {
        return 0;
    }
    do
        id = rand();
    while (id < 1);

    pos = sprintf(line, "%d,%s,%d,%d,%d,%d,%d,%d,", id, name,
                  board1->targets_left, board1->hits, board1->shots,
                  board2->targets_left, board2->hits, board2->shots);

    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            line[pos++] = '0' + board1->grid[y][x];
        }
    }

    line[pos++] = ',';

    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            line[pos++] = '0' + board2->grid[y][x];
        }
    }

    if (fprintf(savefile, "%s\n", line) < 1) {
        fclose(savefile);
        return 0;
    } else {
        fclose(savefile);
        return 1;
    }
}