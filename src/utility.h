#pragma once

/*
 * Contains utility functions:
 * - reading a menu choice/coords/name (with validation),
 * - exiting the application
 * - clearing the screen
 * - "waiting prompt"
 */

// Represents possible user input types inside the gameplay loop
enum input_type {
    INPUT_INVALID = 0,
    INPUT_COORDS,
    INPUT_SAVE,
    INPUT_EXIT
};

// Displays a message and waits for enter before continuing
void wait_enter(void);
// Enter an int from keyboard.
// Note: returns 0 if input was invalid.
// This is taken into account as enums have 0 as invalid variant.
int get_choice(void);
// Reads user input and parses accordingly,
// it could be coordinates, exit or invalid.
// Returns what kind of input it was.
// Note: *y_ptr and *x_ptr are updated only in case of success
enum input_type get_turn_input(int* y_ptr, int* x_ptr);
// Reads user input and copies into provided buffer (must be large enough) if
// the name is valid.
// Returns 0 if name was invalid, 1 if valid.
int get_name(char* name);
// Exit the game with return code and display some message.
void exit_app(int code);
// Clear the terminal screen.
void clear_screen(void);
