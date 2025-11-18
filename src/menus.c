#include <stdio.h>

#include "board.h"
#include "menus.h"
#include "save-load.h"
#include "utility.h"

// LOCAL:

// the logo displayed in the main menu
// Original is in extra/battleship_figlet.txt
const char* BATTLESHIP_LOGO =
    " ____        _   _   _          _     _            \n"
    "| __ )  __ _| |_| |_| | ___ ___| |__ (_)_ __       \n"
    "|  _ \\ / _` | __| __| |/ _ / __| '_ \\| | '_ \\   \n"
    "| |_) | (_| | |_| |_| |  __\\__ | | | | | |_) |    \n"
    "|____/ \\__,_|\\__|\\__|_|\\___|___|_| |_|_| .__/  \n"
    "                                       |_|         \n";

const char* MAIN_MENU_CHOICES = "1.) Play a new game \n"
                                "2.) Load already saved game \n"
                                "3.) Exit the game \n";

const char* LOAD_MENU_CHOICES =
    "1.) List all saved games \n"
    "2.) List all saved games for a particular player \n"
    "3.) Show the board for one of the saved games \n"
    "4.) Load a game \n"
    "5.) Return to main menu \n";

enum turn {
    TURN_NONE = 0,
    TURN_PLAYER,
    TURN_COMPUTER,
    // required before a computer turn to ask user for enter
    // and not immediately make computer turn(s)
    TURN_PASSING,
};

// EXPOSED in menu.c:

enum choice_main_menu run_main_menu(void) {
    int choice = CHOICE_MM_NONE;

    clear_screen();
    printf("%s%s", BATTLESHIP_LOGO, MAIN_MENU_CHOICES);
    choice = get_choice();

    while (choice < CHOICE_MM_NEW_GAME || choice > CHOICE_MM_EXIT) {
        clear_screen();
        printf("%s%s", BATTLESHIP_LOGO, MAIN_MENU_CHOICES);

        printf("You made an invalid choice, try again.\n");
        choice = get_choice();
    }

    return choice;
}

enum choice_load_menu run_load_menu(void) {
    int choice = CHOICE_LM_NONE;

    clear_screen();
    printf("%s", LOAD_MENU_CHOICES);
    choice = get_choice();

    while (choice < CHOICE_LM_LIST_ALL || choice > CHOICE_LM_RETURN) {
        clear_screen();
        printf("%s", LOAD_MENU_CHOICES);

        printf("You made an invalid choice, try again.\n");
        choice = get_choice();
    }

    return choice;
}

// Main gameplay occurs here.
//
// exit_game(1) in `default:` cases means that other enum variants shall never
// be received there. It implies either memory corruption or changes to a
// function broke this invariant and it should be fixed.
//
// extra_message is used to store a response to turns, a variable is needed
// because it is actually printed in the next iteration and between the boards
// and input prompt.
void run_new_game(void) {
    const char* extra_message = "\n";
    int y = -1, x = -1;
    enum turn whose_turn = TURN_PLAYER;
    board_t player_board, computer_board;

    init_board(&player_board);
    init_board(&computer_board);
    // The main gameplay loop.
    // All conitnue`s inside refer to this while ("skip to next turn").
    while (1) {
        // Boards, stats, and extra message are printed in either case
        clear_screen();
        print_2_boards(&player_board, &computer_board, "Your side",
                       "Opponent's side", 1);
        printf("\n");
        printf("%s", extra_message);
        // Then it diverges based on whose turn it is.
        switch (whose_turn) {
        // Buffer turn to give the player a chance to see changes after each
        // turn, otherwise all automatic turns would happen instantly.
        case TURN_PASSING:
            wait_enter();
            whose_turn = TURN_COMPUTER;
            break;
        // Player's turn where coordinates, "exit"/"quit" or "save" could be
        // inputed. All these cases are handled here.
        case TURN_PLAYER:
            printf("Your turn (coords, , \"save\", \"exit\"): ");

            switch (get_turn_input(&y, &x)) {
            case INPUT_INVALID:
                extra_message = "Not a valid turn, try again.\n";
                continue;
            case INPUT_EXIT:
                return;
            case INPUT_SAVE:
                if (save_game(&player_board, &computer_board,
                              "save_from_game")) {
                    extra_message = "The game has been saved.\n";
                } else {
                    extra_message = "The game could not be saved.\n";
                }
                continue;
            case INPUT_COORDS:
                switch (take_shot(&computer_board, y, x)) {
                case SHOT_HIT:
                    extra_message = "You hit the ship. Take an extra turn.\n";
                    continue;
                case SHOT_ALREADY:
                    extra_message = "You already fired at this tile "
                                    "before. Try again.\n";
                    continue;
                case SHOT_INVALID:
                    extra_message =
                        "Invalid coordinates to fire at. Try again\n";
                    continue;
                case SHOT_SHIP_DOWN:
                    extra_message = "You sunk the ship! Take an extra turn.\n";
                    continue;
                case SHOT_MISSED:
                    extra_message = "You hit the water (a miss).\n";
                    whose_turn = TURN_PASSING;
                    continue;
                case SHOT_WIN:
                    clear_screen();
                    print_2_boards(&player_board, &computer_board,
                                   "Your side (Survived)",
                                   "Opponent's side (Destroyed)", 1);
                    printf("\nCongratulations! You win!\n");
                    wait_enter();
                    return;
                default:
                    exit_game(1);
                    break;
                } // switch (take_shot())
                break;
            default:
                exit_game(1);
                break;
            } // switch (get_turn_input())
            break;
            // Computer's turn. It is only reachable if player shot validly but
            // hit water. (and vice versa)
            // Note that SHOT_ALREADY and SHOT_INVALID should never occur here.
        case TURN_COMPUTER:
            switch (computer_take_shot(&player_board)) {
            case SHOT_HIT:
                extra_message =
                    "Opponent hit the ship and takes an extra turn.\n";
                whose_turn = TURN_PASSING;
                continue;
            case SHOT_SHIP_DOWN:
                extra_message =
                    "Opponent sunk the ship and takes an extra turn.\n";
                whose_turn = TURN_PASSING;
                continue;
            case SHOT_MISSED:
                extra_message = "Opponent hit the water (a miss). \n";
                whose_turn = TURN_PLAYER;
                continue;
            case SHOT_WIN:
                clear_screen();
                print_2_boards(&player_board, &computer_board,
                               "Your side (Destroyed)",
                               "Opponent's side (Survived)", 1);
                printf("\nOppenent wins! Better luck next time.\n");
                wait_enter();
                return;
            default:
                exit_game(1);
                break;
            } // switch (computer_take_shot()
            break;
        default:
            exit_game(1);
            break;
        } // switch (whose_turn)
    }
}