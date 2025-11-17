#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "utility.h"

// EXPOSED in utility.h:

// reads all extra characters left in stdin until end of line or EOF
void flush_stdin(void) {
    if (feof(stdin)) {
        clearerr(stdin);
        return;
    }

    int c; // declared int so that it can catch EOF (-1)
    do
        c = getchar();
    while (c != EOF && c != '\n');
}

// returns 0 if input was invalid
int get_choice(void) {
    int choice = 0;
    char* line = NULL;
    size_t length = 0;

    // to fix a problem with entering C-D
    if (feof(stdin)) {
        clearerr(stdin);
    }

    printf("Your choice: ");

    if (getline(&line, &length, stdin) == -1) {
        if (feof(stdin)) {
            clearerr(stdin);
        }
        free(line);
        return 0;
        // -1 (==EOF) - couldn't read at all, 0 - didn't find a %d
    } else if (sscanf(line, " %d", &choice) < 1) {
        free(line);
        return 0;
    } else {
        free(line);
        return choice;
    }
}

// uses getline (internally uses malloc) to read a whole line and then parses
// (must free in the end) exit: containg "exit" or "quit" in the beginning of
// input (case sensitive) coords: "a4", "g7", etc. (case insensitive)
enum input_type get_turn_input(int* y_ptr, int* x_ptr) {
    char* line = NULL;
    size_t length = 0;
    ssize_t bytes_read = 0;

    // to fix a problem with entering C-D
    if (feof(stdin)) {
        clearerr(stdin);
    }

    bytes_read = getline(&line, &length, stdin);

    // check for attempt to exit
    if (bytes_read >= 4 &&
        (strncmp(line, "exit", 4) == 0 || strncmp(line, "quit", 4) == 0)) {
        free(line);
        return INPUT_EXIT;
        // right amount of chars for coords
    } else if ((bytes_read >= 3 && isspace(line[2])) || bytes_read == 2) {
        char first = line[0], second = line[1];
        free(line);

        // right types of 2 chars for coords
        if (isalpha(first) && isdigit(second)) {
            *y_ptr = toupper(first) - 'A';
            *x_ptr = second - '1';
            return INPUT_COORDS;
        } else { // wrong types of 2 chars
            return INPUT_INVALID;
        }
    } else { // error to read or invalid
        free(line);
        return INPUT_INVALID;
    }
}

void exit_game(int code) {
    if (code == 0) {
        printf("Exit\n");
    } else {
        printf("Exit (critical error)\n");
    }
    exit(code);
}

void clear_screen(void) {
    printf("\x1B[2J\x1B[1;1H");
}