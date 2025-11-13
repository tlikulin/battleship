#include <stdio.h>
#include <stdlib.h>

#include "utility.h"

// reads all extra characters left in stdin until end of line or EOF
void flush_stdin(void) {
    int c;
    do 
        c = getchar();
    while (c != EOF && c != '\n');
}

// TODO: replace scanf
int get_int(void) {
    int choice;
    printf("Your choice: ");
    scanf(" %d", &choice);
    flush_stdin();
    return choice;
}

void exit_game(int code) {
    if (code == 0) {
        printf("Exit\n");
    } else {
        printf("Exit (error)\n");
    }
    exit(code);
}