#pragma once

/*
* Contains utility functions such as 
* - reading an int/coords (with validation),
* - exiting the application
*/

// represents possible user input types inside the gameplay loop
enum turn_type {
    TURN_INVALID = 0,
    TURN_COORDS,
    TURN_EXIT
};

// Reads all remaining characters from stdin, so that future reads from it don't get leftover characters
void flush_stdin(void);
// Enter an int from keyboard
int get_int(void);
// reads user input and parses accordingly,
// it could be coordinates, exit or invalid
// returns a value from enum turn_type
// NB: *y_ptr and *x_ptr are updated only in case of success
enum turn_type get_turn_input(int* y_ptr, int* x_ptr);
// Exit the game with return code and message
void exit_game(int code);
// Clear the terminal screen
void clear_screen(void);