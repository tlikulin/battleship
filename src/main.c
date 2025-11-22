#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "menus.h"
// #include "save-load.h"
#include "utility.h"

int main(void) {
    int is_in_load_menu = 0;

    // Forbid using the game non-interactively.
    if (!isatty(0)) {
        printf("This app is for interactive usage only (no pipe)\n");
        exit_app(1);
    }

    // rand() is later used for generating ids,
    // ships and computer shots.
    srand(time(NULL));

    // main app loop (infinite)
    while (1) {
        switch (run_main_menu()) {
        case CHOICE_MM_EXIT:
            exit_app(0);
            break;
        case CHOICE_MM_NEW_GAME:
            play_new_game();
            break;
        case CHOICE_MM_LOAD:
            is_in_load_menu = 1;
            while (is_in_load_menu) {
                switch (run_load_menu()) {
                // TODO: Do actual loading
                case CHOICE_LM_LIST_ALL:
                    print_all_saves();
                    continue;
                case CHOICE_LM_LIST_PLAYER:
                    print_player_saves();
                    continue;
                case CHOICE_LM_LOAD_GAME:
                    if (play_saved_game()) {
                        is_in_load_menu = 0;
                    }
                    continue;
                case CHOICE_LM_SHOW_BOARD:
                    print_saved_board();
                    continue;
                case CHOICE_LM_RETURN:
                    is_in_load_menu = 0;
                    continue;
                default:
                    exit_app(1);
                    break;
                } // switch (run_load_menu())
            }
            break;
        default:
            exit_app(1);
            break;
        } // switch (run_main_menu())
    }
}