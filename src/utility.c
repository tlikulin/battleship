#include <ctype.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"

// LOCAL:

// Reads all extra characters left in stdin until end of line or EOF.
// Currently only used for wait_enter(), other input functions
// use getline(), so they already read to end of line.
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

// EXPOSED in utility.h:

void wait_enter(void) {
    printf("Press enter to continue");
    flush_stdin(); // wait for enter and discard all
}

// returns 0 if input was invalid
int get_choice(void) {
    long choice = 0;
    char* line = NULL;
    char* end_ptr = NULL;
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
    }

    choice = strtol(line, &end_ptr, 10);
    // no integer found or extra stuff afterwards
    if (line == end_ptr || (*end_ptr != '\0' && *end_ptr != '\n')) {
        free(line);
        return 0;
    }
    free(line);
    // cannot fit in an int
    if (choice > INT_MAX || choice < INT_MIN) {
        return 0;
    }

    return choice;
}

// uses getline (internally uses malloc) to read a whole line and then parses
// (must free in the end) exit: containg "exit" or "quit" in the beginning of
// input (case sensitive) coords: "a4", "g7", etc. (case insensitive)
enum input_type get_turn_input(int* y_ptr, int* x_ptr) {
    char* line = NULL;
    size_t length;
    int bytes_read;

    // to fix a problem with entering C-D
    if (feof(stdin)) {
        clearerr(stdin);
    }

    bytes_read = getline(&line, &length, stdin);

    // check for attempt to exit
    if (bytes_read >= 4 &&
        (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0 ||
         strcmp(line, "exit\n") == 0 || strcmp(line, "quit\n") == 0)) {
        free(line);
        return INPUT_EXIT;
        // check for attempt to save game
    } else if (bytes_read >= 4 &&
               (strcmp(line, "save") == 0 || strcmp(line, "save\n") == 0)) {
        free(line);
        return INPUT_SAVE;
        // right amount of chars for coords
    } else if ((bytes_read >= 3 && line[2] == '\n') || bytes_read == 2) {
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

int get_name(char* name) {
    char *line = NULL, c;
    size_t length = 0;
    int bytes_read = 0;

    // to fix a problem with entering C-D
    if (feof(stdin)) {
        clearerr(stdin);
    }

    bytes_read = getline(&line, &length, stdin);

    if (bytes_read == -1) {
        free(line);
        return 0;
    }

    // locate newline (if present), adjust read characters amount
    for (int i = 0; line[i] != '\0' && i <= 21; ++i) {
        if (line[i] == '\n') {
            line[i] = '\0';
            bytes_read = i;
            break;
        }
    }

    // name must have [1;20] characters
    if (bytes_read == 0 || bytes_read > 20) {
        free(line);
        return 0;
    }

    // validate all characters in the name
    // (must be a-zA-Z0-9_-)
    for (int i = 0; i < bytes_read; ++i) {
        c = line[i];

        if (!isalnum(c) && c != '-' && c != '_') {
            free(line);
            return 0;
        }
    }

    strcpy(name, line);
    free(line);
    return 1;
}

void exit_app(int code) {
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