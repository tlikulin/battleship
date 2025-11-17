#pragma once

/*
 * Contains utility functions such as
 * - reading an int/coords (with validation),
 * - exiting the application
 */

// represents possible user input types inside the gameplay loop
enum input_type {
    INPUT_INVALID = 0,
    INPUT_COORDS,
    INPUT_EXIT
};

// Displays a message and waits for enter before continuing
void wait_enter(void);
// Enter an int from keyboard.
// Note: return 0 if input was invalid.
// This is taken into account as enums have 0 as invalid variant.
int get_choice(void);
// reads user input and parses accordingly,
// it could be coordinates, exit or invalid
// returns a value from enum turn_type
// NB: *y_ptr and *x_ptr are updated only in case of success
enum input_type get_turn_input(int* y_ptr, int* x_ptr);
// Exit the game with return code and message
void exit_game(int code);
// Clear the terminal screen
void clear_screen(void);