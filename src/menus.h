#pragma once

/*
 * Provides functionality for displaying menus and getting user choice.
 * Also has blocks of code extracted from main for better comprehension.
 */

/* Main menu:
 * 1.) Play a new game
 * 2.) Load already saved game
 * 3.) Exit the game
 */
// Returned by run_main_menu()
enum choice_main_menu {
    CHOICE_MM_NONE = 0,
    CHOICE_MM_NEW_GAME = 1,
    CHOICE_MM_LOAD = 2,
    CHOICE_MM_EXIT = 3,
};

/* Load menu:
 * 1.) List all saved games
 * 2.) List all saved games for a particular player
 * 3.) Show the board of one of the saved games
 * 4.) Load a game
 * 5.) Return to main menu
 */
// Returned by run_load_menu()
enum choice_load_menu {
    CHOICE_LM_NONE = 0,
    CHOICE_LM_LIST_ALL = 1,
    CHOICE_LM_LIST_PLAYER = 2,
    CHOICE_LM_SHOW_BOARD = 3,
    CHOICE_LM_LOAD_GAME = 4,
    CHOICE_LM_RETURN = 5,
};

// Display the main menu and ask for input until a valid choice is entered.
// Returns that choice.
enum choice_main_menu run_main_menu(void);
// Display the load menu and ask for input until a valid choice is entered.
// Returns that choice.
enum choice_load_menu run_load_menu(void);
// Initiate a new game and make turns until end of game or exit.
void play_new_game(void);
// Read the savefile and display summary of each found game.
void print_all_saves(void);
// Ask for a username and then displays summary of all found games which have
// that player.
void print_player_saves(void);
// Ask for ID and display the saved state of that game.
void print_saved_board(void);
// Load a game by ID and play it until end of game or exit.
// Returns whether the game could be loaded and thus whether to go back to main
// menu when the loaded game finishes.
int play_saved_game(void);
