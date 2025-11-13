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
int get_coords(int*, int*);
// Exit the game with a message
void exit_game(int);