#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "save-load.h"

// EXPOSED in saves.h:

const char* SAVEFILE_NAME = "battleship-savefile.txt";

// Reads a line from the file using getline() - must free.
// Names (first 2 fields) are read byte by byte until a comma is found or name
// becomes too long. The rest is scanned by sscanf().
int read_next_save(FILE* file, int* id, board_t* board1, board_t* board2) {
    char* line = NULL;
    size_t length = 0;
    int bytes_read, offset1, offset2;
    char str_grid1[GRID_SIZE * GRID_SIZE + 1] = {0};
    char str_grid2[GRID_SIZE * GRID_SIZE + 1] = {0};
    char tile1, tile2;

    if (file == NULL) {
        return -1;
    }

    bytes_read = getline(&line, &length, file);
    if (bytes_read == EOF) {
        free(line);
        return -1;
        // line too short (a rough lower bound)
    } else if (bytes_read < 2 * GRID_SIZE * GRID_SIZE + 10) {
        free(line);
        return 0;
    }

    // Reading the names, it is variable length so requires special treatment.
    for (offset1 = 0; line[offset1] != ','; ++offset1) {
        // name too long
        if (offset1 == NAME_LEN) {
            free(line);
            return 0;
        }

        board1->name[offset1] = line[offset1];
    }
    board1->name[offset1] = '\0'; // terminate name
    ++offset1;                    // consume comma
    for (offset2 = 0; line[offset1 + offset2] != ','; ++offset2) {
        // name too long
        if (offset2 == NAME_LEN) {
            free(line);
            return 0;
        }

        board2->name[offset2] = line[offset1 + offset2];
    }
    board2->name[offset2] = '\0'; // terminate name
    ++offset2;                    // consume comma

    // reading the rest of the saved game
    if (sscanf(line + offset1 + offset2, "%d,%d,%d,%d,%d,%d,%d,%64s,%64s", id,
               &board1->targets_left, &board1->hits, &board1->shots,
               &board2->targets_left, &board2->hits, &board2->shots, str_grid1,
               str_grid2) < 9) {
        free(line);
        return 0;
    }

    free(line);

    // copies grid tiles character by character
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            tile1 = str_grid1[y * GRID_SIZE + x];
            tile2 = str_grid2[y * GRID_SIZE + x];

            if (!isdigit(tile1) || !isdigit(tile2)) {
                return 0;
            }

            board1->grid[y][x] = tile1 - '0';
            board2->grid[y][x] = tile2 - '0';
        }
    }

    return 1;
}

// Write everything to a buffer first, then writes the line to the file in 1 go.
int save_game(board_t* board1, board_t* board2) {
    char line[512] = {0};
    int pos, id;
    FILE* savefile = fopen(SAVEFILE_NAME, "a");
    if (savefile == NULL) {
        return 0;
    }
    do // accept positive numbers only
        id = rand();
    while (id < 1);

    pos =
        sprintf(line, "%s,%s,%d,%d,%d,%d,%d,%d,%d,", board1->name, board2->name,
                id, board1->targets_left, board1->hits, board1->shots,
                board2->targets_left, board2->hits, board2->shots);

    // translates grid character by character
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

// Reads saves into temporary variables. If right ID is found, copies temporary
// values to provided pointers and finishes
int load_game(int id, board_t* board1, board_t* board2) {
    board_t temp_board1, temp_board2;
    int status, temp_id;
    FILE* savefile = fopen(SAVEFILE_NAME, "r");
    if (savefile == NULL) {
        return 0;
    }

    while ((status = read_next_save(savefile, &temp_id, &temp_board1,
                                    &temp_board2)) != -1) {
        if (status == 1 && temp_id == id) {
            *board1 = temp_board1;
            *board2 = temp_board2;
            fclose(savefile);
            return 1;
        }
    }

    fclose(savefile);
    return 0;
}
