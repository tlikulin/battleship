#pragma once

/*
* Contains utility functions such as 
* - reading an int/coords (with validation),
* - exiting the application
*/

// Enter an int from keyboard
int get_int(void);
// Enter coords from keyboard and store at pointers, 
// returns 0 if succeded, 1 if invalid input
int get_coords(int* y_ptr, int* x_ptr);
// Exit the game with return code and message
void exit_game(int code);
// Clear the terminal screen
void clear_screen(void);