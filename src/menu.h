#pragma once

/*
 * Provides functionality for displaying menus and getting user choice.
 */

enum choice_main_menu {     // Main menu:
    CHOICE_MM_NONE = 0,     //
    CHOICE_MM_NEW_GAME = 1, // 1.) Play a new game
    CHOICE_MM_LOAD = 2,     // 2.) Load already saved game
    CHOICE_MM_EXIT = 3,     // 3.) Exit the game
};

/* Load menu:
 * 1. List all saved games
 * 2. List all saved games for a particular player
 * 3. Show the board of one of the saved games
 * 4. Load a game
 * 5. Return to main menu
 */
enum choice_load_menu {
    CHOICE_LM_NONE = 0,
    CHOICE_LM_LIST_ALL,
    CHOICE_LM_LIST_PLAYER,
    CHOICE_LM_LOAD_GAME,
    CHOICE_LM_SHOW_BOARD,
    CHOICE_LM_RETURN,
};

// display the main menu and ask for input until a valid choice is entered
int run_main_menu(void);