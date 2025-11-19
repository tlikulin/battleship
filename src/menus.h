#pragma once

/*
 * Provides functionality for displaying menus and getting user choice.
 * Also has loops extracted from main for better comprehension.
 */

/* Main menu:
 * 1.) Play a new game
 * 2.) Load already saved game
 * 3.) Exit the game
 */
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
enum choice_load_menu {
    CHOICE_LM_NONE = 0,
    CHOICE_LM_LIST_ALL = 1,
    CHOICE_LM_LIST_PLAYER = 2,
    CHOICE_LM_SHOW_BOARD = 3,
    CHOICE_LM_LOAD_GAME = 4,
    CHOICE_LM_RETURN = 5,
};

// display the main menu and ask for input until a valid choice is entered
enum choice_main_menu run_main_menu(void);
// display the load menu and ask for input until a valid choice is entered
enum choice_load_menu run_load_menu(void);
// initiate a new game and make turns until end of game or exit
void play_new_game(void);
//
void play_saved_game(void);
//
void print_saved_board(void);
//
void print_all_saves(void);
//
void print_player_saves(void);