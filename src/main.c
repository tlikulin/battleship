#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(__unix__) || defined(__unix)
#include <unistd.h>
#endif

#include "menus.h"
#include "utility.h"

/*
 * This is the file which contains main().
 * There is only logic relating to branching
 * based on main/load menu choice.
 *
 * The implementations itself are located in separate files.
 * A header file only contains type definitions and prototypes of functions used
 * by other modules, while the corresponding source file contains bodies of
 * those functions plus internal helpers.
 *
 * Namely:
 * board: board initialization, printing, updating (shots).
 * menus: displaying and getting choice from main/load menu, also a function for
 *        each menu choice.
 * save-load: saving state of the game to the file and reading
 *            the save back.
 * utility: getting user input (with validation) and misc.
 */

int main(void) {
    int is_in_load_menu = 0;

#if defined(__unix__) || defined(__unix)
    // Forbid using the game non-interactively.
    if (!isatty(0)) {
        printf("This app is for interactive usage only (no pipe)\n");
        exit_app(1);
    }
#endif

    // rand() is later used for generating IDs,
    // ships and computer shots.
    srand(time(NULL));

    // infinite main loop
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