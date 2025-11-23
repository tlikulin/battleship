#include <ctype.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"

// LOCAL:

#define BUFFER_SIZE 512

// Reads all extra characters left in stdin until end of line or EOF.
void flush_stdin(void) {
    // to fix a problem with entering C-D
    if (feof(stdin)) {
        clearerr(stdin);
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
    char line[BUFFER_SIZE];
    char* end_ptr = NULL;

    // to fix a problem with entering C-D
    if (feof(stdin)) {
        clearerr(stdin);
    }

    printf("Your choice: ");

    if (fgets(line, BUFFER_SIZE, stdin) == NULL) {
        if (feof(stdin)) {
            clearerr(stdin);
        }
        return 0;
    }

    choice = strtol(line, &end_ptr, 10);
    // no integer found or extra stuff afterwards
    if (line == end_ptr || (*end_ptr != '\0' && *end_ptr != '\n')) {
        return 0;
    }
    // cannot fit in an int
    if (choice > INT_MAX || choice < INT_MIN) {
        return 0;
    }

    return choice;
}

// Uses fgets() to read a whole line and then parses.
// (must free in the end)
// coords: "a4", "g7", etc. (case insensitive)
// save: "save" (case sensitive)
// exit: "exit" or "quit" (case sensitive)
enum input_type get_turn_input(int* y_ptr, int* x_ptr) {
    char line[BUFFER_SIZE] = {0};

    // to fix a problem with entering C-D
    if (feof(stdin)) {
        clearerr(stdin);
    }

    if (fgets(line, BUFFER_SIZE, stdin) == NULL) {
        return INPUT_INVALID;
    }

    // check for attempt to exit
    if ((strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0 ||
         strcmp(line, "exit\n") == 0 || strcmp(line, "quit\n") == 0)) {
        return INPUT_EXIT;
        // check for attempt to save game
    } else if ((strcmp(line, "save") == 0 || strcmp(line, "save\n") == 0)) {
        return INPUT_SAVE;
        // right amount of chars for coords
    } else if (line[2] == '\n' || line[2] == '\0') {
        char first = line[0], second = line[1];

        // right types of 2 chars for coords
        if (isalpha(first) && isdigit(second)) {
            *y_ptr = toupper(first) - 'A';
            *x_ptr = second - '1';
            return INPUT_COORDS;
        } else { // wrong types of 2 chars
            return INPUT_INVALID;
        }
    } else {
        return INPUT_INVALID;
    }
}

// Name must be 20 characters maximum (excluding new line) and have only
// specified characters (alphanum + -_).
int get_name(char* name) {
    char line[BUFFER_SIZE], c;
    int length;

    // to fix a problem with entering C-D
    if (feof(stdin)) {
        clearerr(stdin);
    }

    if (fgets(line, BUFFER_SIZE, stdin) == NULL) {
        return 0;
    }

    length = strlen(line);

    // locate newline (if present), strip it and adjust read characters amount
    for (int i = 0; line[i] != '\0' && i <= 20; ++i) {
        if (line[i] == '\n') {
            line[i] = '\0';
            length = i;
            break;
        }
    }

    // name must have [1;20] characters
    if (length < 1 || length > 20) {
        return 0;
    }

    // name "Computer" is prohibited to use by a real player
    if (strcmp(line, "Computer") == 0) {
        return 0;
    }

    // validate all characters in the name
    // (must be a-zA-Z0-9_-)
    for (int i = 0; i < length; ++i) {
        c = line[i];

        if (!isalnum(c) && c != '-' && c != '_') {
            return 0;
        }
    }

    strcpy(name, line);
    return 1;
}

// Used to exit otherwise infinite loops.
// Normally, only 0 is expected.
void exit_app(int code) {
    if (code == 0) {
        printf("Exit\n");
    } else {
        printf("Exit (critical error)\n");
    }
    exit(code);
}

// Uses ANSI escape codes to clear screen and move cursos to the top-left
// corner.
void clear_screen(void) {
    printf("\x1B[2J\x1B[1;1H");
}
