#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>

#include <stdio.h>

#include "utility.h"

// LOCAL:

// reads all extra characters left in stdin until end of line or EOF
void flush_stdin(void) {
    int c;
    do 
        c = getchar();
    while (c != EOF && c != '\n');
}

// EXPOSED in utility.h:

// TODO: replace scanf
int get_int(void) {
    int choice;
    printf("Your choice: ");
    scanf(" %d", &choice);
    flush_stdin();
    return choice;
}

int get_coords(int* y_ptr, int* x_ptr) {
    char* line = NULL;
    size_t length = 0;
    ssize_t bytes_read = 0;

    if (feof(stdin)) clearerr(stdin); // to fix a problem when entering C-D
    bytes_read = getline(&line, &length, stdin);

    if ((bytes_read == 3 && line[2] == '\n') || (bytes_read == 2 && line[2] == '\0')) { // right amount of chars
        char first = line[0], second = line[1];
        free(line);

        if (isalpha(first) && isdigit(second)) { // right types of 2 chars
            *y_ptr = toupper(first) - 'A';
            *x_ptr = second - '1';
            return 0;
        } 
        else { // wrong types of 2 chars
            return 1;
        }
    } 
    else { // wrong amount of chars or error to read
        free(line);
        return 1;
    }
}

void exit_game(int code) {
    if (code == 0) {
        printf("Exit\n");
    } 
    else {
        printf("Exit (critical error)\n");
    }
    exit(code);
}

void clear_screen(void) {
    printf("\x1B[2J\x1B[1;1H");
}