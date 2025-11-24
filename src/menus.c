#include <stdio.h>
#include <string.h>

#include "board.h"
#include "menus.h"
#include "save-load.h"
#include "utility.h"

// LOCAL:

// The components of main and load menus.
// Backslahes are to be escaped, so not aligned.
// (originals are in extra/figlet.txt)

const char* BATTLESHIP_LOGO =
    " ____        _   _   _          _     _            \n"
    "| __ )  __ _| |_| |_| | ___ ___| |__ (_)_ __       \n"
    "|  _ \\ / _` | __| __| |/ _ / __| '_ \\| | '_ \\   \n"
    "| |_) | (_| | |_| |_| |  __\\__ | | | | | |_) |    \n"
    "|____/ \\__,_|\\__|\\__|_|\\___|___|_| |_|_| .__/  \n"
    "                                       |_|         \n";

const char* LOAD_LOGO = " _                    _        \n"
                        "| |    ___   __ _  __| |       \n"
                        "| |   / _ \\ / _` |/ _` |      \n"
                        "| |__| (_) | (_| | (_| |       \n"
                        "|_____\\___/ \\__,_|\\__,_|    \n";

const char* MAIN_MENU_CHOICES = "1.) Play a new game \n"
                                "2.) Load already saved game \n"
                                "3.) Exit the game \n";

const char* LOAD_MENU_CHOICES =
    "1.) List all saved games \n"
    "2.) List all saved games for a particular player \n"
    "3.) Show the board for one of the saved games \n"
    "4.) Load a game \n"
    "5.) Return to main menu \n";

// Used to represent whose turn it is along with clear transfer of turn.
enum turn {
    TURN_NONE = 0,
    TURN_PLAYER,
    TURN_COMPUTER,
    // required before a computer turn to ask user for enter
    // and not immediately make computer turn(s)
    TURN_PASSING,
};

// Main gameplay occurs here. Boards must be initialised before it is called.
// This allows both new game and loaded game to use the same loop.
//
// exit_app(1) in `default:` cases means that other enum variants shall never
// be received there. It implies either memory corruption occured or changes to
// a function broke this invariant and it should be fixed.
//
// extra_message is used to store a response to turns, a variable is needed
// because it is actually printed in the next iteration and between the boards
// and input prompt.
//
// Invalid turn/input does not result in passing the turn to the opponent.
void run_game(board_t* player_board, board_t* computer_board) {
    const char* extra_message = "\n";
    int y = -1, x = -1;
    enum turn whose_turn = TURN_PLAYER;

    // The main gameplay loop.
    // All conitnue`s inside refer to this while ("skip to next turn").
    // The only way to exit it is to return from the function.
    while (1) {
        // Boards, stats, and extra message are printed in either case
        clear_screen();
        print_boards(player_board, computer_board, 1);
        putchar('\n');
        printf("%s", extra_message);
        // Then it diverges based on whose turn it is.
        switch (whose_turn) {
        // Buffer turn to give the player a chance to see changes after each
        // computer turn, otherwise all automatic turns would happen instantly.
        case TURN_PASSING:
            wait_enter();
            whose_turn = TURN_COMPUTER;
            break;
        // Player's turn where coordinates, "exit"/"quit" or "save" could be
        // inputed. All these cases are handled here.
        case TURN_PLAYER:
            printf("Your turn (coords/\"save\"/\"exit\"): ");

            switch (get_turn_input(&y, &x)) {
            case INPUT_INVALID:
                extra_message = "Not a valid turn, try again.\n";
                continue;
            case INPUT_EXIT:
                return;
            case INPUT_SAVE:
                if (save_game(player_board, computer_board)) {
                    extra_message = "The game has been saved.\n";
                } else {
                    extra_message = "The game could not be saved.\n";
                }
                continue;
            case INPUT_COORDS:
                switch (take_shot(computer_board, y, x)) {
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
                    print_boards(player_board, computer_board, 1);
                    printf("\nCongratulations! You won!\n");
                    wait_enter();
                    return;
                default:
                    exit_app(1);
                    break;
                } // switch (take_shot())
                break;
            default:
                exit_app(1);
                break;
            } // switch (get_turn_input())
            break;
            // Computer's turn. It is only reachable if player shot validly but
            // hit water. (and vice versa)
            // Note that SHOT_ALREADY and SHOT_INVALID should never occur here.
        case TURN_COMPUTER:
            switch (computer_take_shot(player_board)) {
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
                print_boards(player_board, computer_board, 1);
                printf("\nOppenent won! Better luck next time.\n");
                wait_enter();
                return;
            default:
                exit_app(1);
                break;
            } // switch (computer_take_shot()
            break;
        default:
            exit_app(1);
            break;
        } // switch (whose_turn)
    }
}

// EXPOSED in menu.c:

// Displays logo and choice and asks for input.
// Repeates until some valid choice is found.
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

// Displays logo and choice and asks for input.
// Repeates until some valid choice is found.
enum choice_load_menu run_load_menu(void) {
    int choice = CHOICE_LM_NONE;

    clear_screen();
    printf("%s\n%s", LOAD_LOGO, LOAD_MENU_CHOICES);
    choice = get_choice();

    while (choice < CHOICE_LM_LIST_ALL || choice > CHOICE_LM_RETURN) {
        clear_screen();
        printf("%s", LOAD_MENU_CHOICES);

        printf("You made an invalid choice, try again.\n");
        choice = get_choice();
    }

    return choice;
}

// Initializes player and computer boards.
// Player is asked for the name; if it is invalid, falls back to default
// "Player".
void play_new_game(void) {
    board_t player_board, computer_board;
    char player_name[NAME_LEN + 1];

    clear_screen();
    printf("Enter your (user)name.\n");
    printf("Letters, digits, '-', and '_' only; 20 characters max.\n");
    printf("> ");

    init_board(&player_board, get_name(player_name) ? player_name : "Player");
    init_board(&computer_board, "Computer");

    run_game(&player_board, &computer_board);
}

// Tries to load game with entered ID, it is played if successful.
int play_saved_game(void) {
    board_t player_board, computer_board;
    printf("Enter the ID of the game to load.\n\n");
    int id = get_choice();

    if (id <= 0) {
        printf("Invalid ID: must be a positive integer\n");
        wait_enter();
        return 0;
    } else if (load_game(id, &player_board, &computer_board) == 0) {
        printf("\nGame with this ID has not been found.\n");
        wait_enter();
        return 0;
    } else {
        run_game(&player_board, &computer_board);
        return 1;
    }
}

// Tries to load game with entered ID, displays it if successful.
void print_saved_board(void) {
    board_t player_board, computer_board;
    printf("\nEnter the ID of the game to load.\n");
    int id = get_choice();

    if (id <= 0) {
        printf("Invalid ID: must be a positive integer\n");
    } else if (load_game(id, &player_board, &computer_board) == 0) {
        printf("\nGame with this ID has not been found.\n");
    } else {
        clear_screen();
        print_boards(&player_board, &computer_board, 1);
        putchar('\n');
    }
    wait_enter();
}

// Reads each line of the savefile and displays summary.
void print_all_saves(void) {
    board_t board1, board2;
    int id, status;
    FILE* savefile = fopen(SAVEFILE_NAME, "r");
    if (savefile == NULL) {
        printf("Error: cannot find the savefile `%s`\n", SAVEFILE_NAME);
        wait_enter();
        return;
    }

    clear_screen();
    printf("Listing all games saved in the file:\n\n");

    while ((status = read_next_save(savefile, &id, &board1, &board2)) != -1) {
        if (status == 1) {
            printf("ID %d:\t%s (made %d hits/%d shots) vs. %s (made %d hits/%d "
                   "shots)\n",
                   id, board1.name, board2.hits, board2.shots, board2.name,
                   board1.hits, board1.shots);
        }
    }

    fclose(savefile);
    putchar('\n');
    wait_enter();
}

// Reads each line of the savefile and displays summary if the entered name
// matches.
void print_player_saves(void) {
    board_t board1, board2;
    char search_name[NAME_LEN + 1];
    int id, status;
    FILE* savefile = fopen(SAVEFILE_NAME, "r");
    if (savefile == NULL) {
        printf("Error: cannot find the savefile `%s`\n", SAVEFILE_NAME);
        wait_enter();
        return;
    }

    printf("\nEnter the name to search for: ");
    if (!get_name(search_name)) {
        printf("Not a valid name.\n");
        fclose(savefile);
        wait_enter();
        return;
    }

    clear_screen();
    printf("Listing all games with player %s:\n\n", search_name);

    while ((status = read_next_save(savefile, &id, &board1, &board2)) != -1) {
        if (status == 1 && (strcmp(search_name, board1.name) == 0 ||
                            strcmp(search_name, board2.name) == 0)) {
            printf("ID %d:\t%s (made %d hits/%d shots) vs. %s (made %d hits/%d "
                   "shots)\n",
                   id, board1.name, board2.hits, board2.shots, board2.name,
                   board1.hits, board1.shots);
        }
    }

    fclose(savefile);
    putchar('\n');
    wait_enter();
}
