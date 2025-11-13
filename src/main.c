#include <stdio.h>

#include "board.h"
#include "menu.h"
#include "utility.h"

int main(void) {
    board_t player_board;
    init_board(&player_board);

    int choice = run_main_menu();
    switch (choice) {
    case CHOICE_MM_EXIT:
        exit_game(0);
        break;
    case CHOICE_MM_NEW_GAME:
        printf("Player's board:\n\n");
        print_board(&player_board);
        printf("\n");
        break;
    default:
        exit_game(1);
        break;
    }
}