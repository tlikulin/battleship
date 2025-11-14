#include <stdio.h>
#include <unistd.h>

#include "board.h"
#include "menu.h"
#include "utility.h"

int main(void) {
    int choice;
    int y = -1, x = -1;
    const char* extra_message = "";
    board_t player_board;

    if (!isatty(0)) {
        printf("Please do not pipe into this\n");
        exit_game(1);
    }

    init_board(&player_board);

    // main loop (infinite)
    while (1) { 
        choice = run_main_menu();
        
        switch (choice) {
        case CHOICE_MM_EXIT:
            exit_game(0);
            break;
        case CHOICE_MM_NEW_GAME:
            extra_message = "\n";
            while (1) {
                clear_screen();
                printf("Player's board:\n\n");
                print_board(&player_board);
                printf("\n");

                printf("%s", extra_message);
                printf("Enter coords: ");
                if (get_coords(&y, &x) == 1) {
                    extra_message = "Invalid input for coordinates, try again.\n";
                    continue;
                }
                
                switch (take_shot(&player_board, y, x)) {
                case SHOT_HIT:
                    extra_message = "You hit the ship.\n";
                    break;
                case SHOT_ALREADY:
                    extra_message = "You already fired at this tile before. Try again.\n";
                    break;
                case SHOT_INVALID:
                    extra_message = "Invalid coordinates to fire at.\n";
                    break;
                case SHOT_SHIP_DOWN:
                    extra_message = "You sunk the ship!\n";
                    break;
                case SHOT_MISSED:
                    extra_message = "You hit the water (a miss). \n";
                    break;
                default:
                    exit_game(1);
                    break;
                }
            }

            break;
        // unexpected, run_main_menu() supposed to allways return one of above
        default:
            exit_game(1);
            break;
        }
    }
}