#include <assert.h>
#include <stdio.h>

#include "board.h"
#include "menu.h"

int main(void) {
    board_t player_board = create_empty_board();

    int choice = run_main_menu();
    switch (choice) {
    case CHOICE_MM_EXIT:
        printf("Exit\n");
        return 0;
    case CHOICE_MM_NEW_GAME:
        printf("Player's board:\n\n");
        print_board(&player_board);
        printf("\n");
        break;
    default:
        printf("Exit (error)\n");
        return 1;
    }
}